#include "engine.h"

#include "project.h"
#include "rendering/vk_utils.h"

const uint32_t DEFAULT_WIDTH = 1920;
const uint32_t DEFAULT_HEIGHT = 1080;
const uint32_t MAX_FRAME_WAIT_TIME = 10000000; // 1 second

/*
 * Do one iteration (frame) worth of stuff
 *
 * @returns Whether the app should continue running
 */
static bool engine_iterate(engine_t* self);

bool engine_init(engine_t* self) {
    if (!window_create(PROJECT_NAME, DEFAULT_WIDTH, DEFAULT_HEIGHT, &self->window)) {
        return false;
    }

    if (!render_context_init(&self->render_context, &self->window)) {
        return false;
    }

    if (!render_device_init(&self->render_device, &self->render_context)) {
        return false;
    }

    if (!swapchain_init(&self->swapchain, &self->render_context, &self->render_device)) {
        return false;
    }

    self->current_frame = 0;
    for (size_t i = 0; i < ENGINE_MAX_FRAMES_IN_FLIGHT; i++) {
        frame_init(self->frames + i, &self->render_device);
    }

    if (!triangle_pipeline_create(
          &self->triangle_pipeline, &self->render_device, self->swapchain.surface_format.format)) {
        return false;
    }

    return true;
}

void engine_destroy(engine_t* self) {
    vkDeviceWaitIdle(self->render_device.vk_device);

    triangle_pipeline_destroy(&self->triangle_pipeline, &self->render_device);

    for (size_t i = 0; i < ENGINE_MAX_FRAMES_IN_FLIGHT; i++) {
        frame_destroy(self->frames + i, &self->render_device);
    }

    swapchain_destroy(&self->swapchain, &self->render_device);
    render_device_destroy(&self->render_device);
    render_context_destroy(&self->render_context);
    window_destroy(&self->window);
}

void engine_run(engine_t* self) {
    bool continue_running = true;
    event_t event;

    while (continue_running) {
        while (continue_running && window_poll_event(&self->window, &event)) {
            continue_running &= event.type != EVENT_TYPE_QUIT;
        }

        if (continue_running) {
            continue_running &= engine_iterate(self);
        }
    }
}

static bool engine_iterate(engine_t* self) {
    self->current_frame = (self->current_frame + 1) % ENGINE_MAX_FRAMES_IN_FLIGHT;
    frame_t* current_frame = self->frames + self->current_frame;

    if (!frame_wait_for_render_completed(
          current_frame, &self->render_device, MAX_FRAME_WAIT_TIME)) {
        return false;
    }

    if (!swapchain_acquire_next_image(
          &self->swapchain, &self->render_device, MAX_FRAME_WAIT_TIME, current_frame)) {
        return false;
    }

    if (self->swapchain.out_of_date) {
        render_device_update_present_capabilities(&self->render_device, &self->render_context);
        swapchain_recreate(&self->swapchain, &self->render_context, &self->render_device);
        return true;
    }

    if (!frame_begin_new(current_frame, &self->render_device)) {
        return false;
    }

    swapchain_prepare_current_image_for_writing(&self->swapchain, current_frame->command_buffer);

    VkRenderingAttachmentInfo color_attachment = {
      .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
      .imageView = self->swapchain.current_image_view,
      .imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
      .clearValue = {clear_color_white_unorm},
    };

    VkRect2D whole_frame_rect = {
      .extent = self->swapchain.extent,
      .offset =
        {
                 .x = 0,
                 .y = 0,
                 },
    };

    VkRenderingInfo rendering_info = {
      .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
      .renderArea = whole_frame_rect,
      .layerCount = 1,
      .colorAttachmentCount = 1,
      .pColorAttachments = &color_attachment,
    };

    vkCmdBeginRendering(current_frame->command_buffer, &rendering_info);

    vkCmdBindPipeline(
      current_frame->command_buffer,
      VK_PIPELINE_BIND_POINT_GRAPHICS,
      self->triangle_pipeline.vk_pipeline);

    VkViewport viewport = {
      .x = 0.0f,
      .y = 0.0f,
      .width = (float)self->swapchain.extent.width,
      .height = (float)self->swapchain.extent.height,
      .minDepth = 0.0f,
      .maxDepth = 1.0f,
    };
    vkCmdSetViewport(current_frame->command_buffer, 0, 1, &viewport);
    vkCmdSetScissor(current_frame->command_buffer, 0, 1, &whole_frame_rect);

    vkCmdDraw(current_frame->command_buffer, 3, 1, 0, 0);

    vkCmdEndRendering(current_frame->command_buffer);

    swapchain_prepare_current_image_for_presentation(
      &self->swapchain, current_frame->command_buffer);

    if (!frame_end(current_frame, &self->render_device)) {
        return false;
    }

    if (!swapchain_present(&self->swapchain, &self->render_device, current_frame)) {
        return false;
    }

    if (self->swapchain.out_of_date || self->swapchain.suboptimal) {
        render_device_update_present_capabilities(&self->render_device, &self->render_context);
        swapchain_recreate(&self->swapchain, &self->render_context, &self->render_device);
    }

    return true;
}
