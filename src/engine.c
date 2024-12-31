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

    if (!swapchain_init(
          &self->swapchain,
          self->render_context.vk_surface,
          &self->render_device,
          &self->render_device.present_capabilities,
          &self->window)) {
        return false;
    }

    self->current_frame = 0;
    for (size_t i = 0; i < ENGINE_MAX_FRAMES_IN_FLIGHT; i++) {
        frame_init(self->frames + i, &self->render_device);
    }

    return true;
}

void engine_destroy(engine_t* self) {
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

    if (!frame_begin_new(current_frame, &self->render_device)) {
        return false;
    }

    swapchain_prepare_current_image_for_writing(&self->swapchain, current_frame->command_buffer);

    vkCmdClearColorImage(
      current_frame->command_buffer,
      self->swapchain.current_image,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      &clear_color_white_unorm,
      1,
      &subresource_range_color_all_mips);

    swapchain_prepare_current_image_for_presentation(
      &self->swapchain, current_frame->command_buffer);

    if (!frame_end(current_frame, &self->render_device)) {
        return false;
    }

    if (!swapchain_present(&self->swapchain, &self->render_device, current_frame)) {
        return false;
    }

    return true;
}
