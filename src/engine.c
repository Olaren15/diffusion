#include "engine.h"

#include "core/time.h"
#include "math.h"
#include "project.h"
#include "rendering/model/mesh.h"
#include "rendering/vk_utils.h"

const uint32_t DEFAULT_WIDTH = 1920;
const uint32_t DEFAULT_HEIGHT = 1080;
const uint32_t MAX_FRAME_WAIT_TIME = 10000000; // 1 second

const vertex_t TRIANGLE_VERTICES[] = {
  {
   .position = {.x = 0.0f, .y = -0.5f, .z = 0.0f},
   .color = {.r = 1.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f},
   },
  {
   .position = {.x = 0.5f, .y = 0.5f, .z = 0.0f},
   .color = {.r = 0.0f, .g = 1.0f, .b = 0.0f, .a = 1.0f},
   },
  {
   .position = {.x = -0.5f, .y = 0.5f, .z = 0.0f},
   .color = {.r = 0.0f, .g = 0.0f, .b = 1.0f, .a = 1.0f},
   }
};

const uint32_t TRIANGLE_INDICES[] = {0, 1, 2};

/*
 * Do one iteration (frame) worth of stuff
 *
 * @returns Whether the app should continue running
 */
static bool engine_iterate(engine_t* self);

bool engine_init(engine_t* self) {
    self->last_timestamp_nanoseconds = get_elapsed_nanoseconds();

    if (!window_create(PROJECT_NAME, DEFAULT_WIDTH, DEFAULT_HEIGHT, &self->window)) {
        return false;
    }

    if (!render_context_init(&self->render_context, &self->window)) {
        return false;
    }

    if (!render_device_init(&self->render_device, &self->render_context)) {
        return false;
    }

    if (!rebar_gpu_allocator_available(&self->render_device)) {
        return false;
    }

    if (!rebar_gpu_allocator_init(&self->gpu_allocator, &self->render_device)) {
        return false;
    }

    if (!swapchain_init(&self->swapchain, &self->render_context, &self->render_device)) {
        return false;
    }

    VkDescriptorSetLayoutBinding scene_data_binding = {
      .binding = 0,
      .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
      .descriptorCount = 1,
      .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
    };

    VkDescriptorSetLayoutCreateInfo layout_create_info = {
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
      .bindingCount = 1,
      .pBindings = &scene_data_binding,
    };

    if (
      vkCreateDescriptorSetLayout(
        self->render_device.vk_device,
        &layout_create_info,
        NULL,
        &self->scene_descriptor_set_layout)
      != VK_SUCCESS) {
        return false;
    }

    VkDescriptorPoolSize descriptor_pool_sizes[1] = {
      {.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = 1}
    };

    VkDescriptorPoolCreateInfo descriptor_pool_create_info = {
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
      .maxSets = ENGINE_MAX_FRAMES_IN_FLIGHT,
      .poolSizeCount = 1,
      .pPoolSizes = descriptor_pool_sizes,
    };

    if (
      vkCreateDescriptorPool(
        self->render_device.vk_device, &descriptor_pool_create_info, NULL, &self->descriptor_pool)
      != VK_SUCCESS) {
        return false;
    }

    self->current_frame = 0;
    for (size_t i = 0; i < ENGINE_MAX_FRAMES_IN_FLIGHT; i++) {
        frame_init(
          self->frames + i,
          &self->render_device,
          &self->gpu_allocator,
          self->scene_descriptor_set_layout,
          self->descriptor_pool);
    }

    if (!triangle_pipeline_create(
          &self->triangle_pipeline,
          &self->render_device,
          self->swapchain.surface_format.format,
          self->scene_descriptor_set_layout)) {
        return false;
    }

    mesh_t triangle_mesh = {
      .vertices = dynamic_array_from((slice_t){
        .first_element = &TRIANGLE_VERTICES,
        .element_count = 3,
        .element_size = sizeof(vertex_t),
      }),
      .indices = dynamic_array_from((slice_t){
        .first_element = &TRIANGLE_INDICES,
        .element_count = 3,
        .element_size = sizeof(uint32_t),
      }),
    };

    gpu_mesh_create(
      &self->triangle_mesh, &triangle_mesh, &self->render_device, &self->gpu_allocator);

    dynamic_array_free(&triangle_mesh.vertices);
    dynamic_array_free(&triangle_mesh.indices);

    return true;
}

void engine_destroy(engine_t* self) {
    vkDeviceWaitIdle(self->render_device.vk_device);

    gpu_mesh_destroy(&self->triangle_mesh, &self->render_device, &self->gpu_allocator);

    triangle_pipeline_destroy(&self->triangle_pipeline, &self->render_device);

    vkDestroyDescriptorPool(self->render_device.vk_device, self->descriptor_pool, NULL);
    vkDestroyDescriptorSetLayout(
      self->render_device.vk_device, self->scene_descriptor_set_layout, NULL);

    for (size_t i = 0; i < ENGINE_MAX_FRAMES_IN_FLIGHT; i++) {
        frame_destroy(self->frames + i, &self->render_device, &self->gpu_allocator);
    }

    swapchain_destroy(&self->swapchain, &self->render_device);
    rebar_gpu_allocator_destroy(&self->gpu_allocator, &self->render_device);
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
    uint64_t current_timestamp_nanoseconds = get_elapsed_nanoseconds();

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

    double current_timestamp_seconds = nanoseconds_to_seconds(current_timestamp_nanoseconds);
    vector_3f_position_t camera_position = {
      .x = (float)sin(current_timestamp_seconds), .y = 0.0f, .z = 0.0f};
    matrix_4x4f_t view = matrix_4x4f_translate(matrix_4x4f_identity, camera_position);
    matrix_4x4f_t projection = matrix_4x4f_identity; // TODO
    scene_data_t scene_data = {
      .view = view,
      .projection = projection,
      .view_projection = matrix_4x4f_multiply(view, projection),
    };
    if (!frame_update_gpu_data(current_frame, &self->render_device, &scene_data)) {
        return false;
    }

    vkCmdBindDescriptorSets(
      current_frame->command_buffer,
      VK_PIPELINE_BIND_POINT_GRAPHICS,
      self->triangle_pipeline.layout,
      0,
      1,
      &current_frame->gpu_data.descriptor_set,
      0,
      NULL);

    swapchain_prepare_current_image_for_writing(&self->swapchain, current_frame->command_buffer);

    VkRenderingAttachmentInfo color_attachment = {
      .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
      .imageView = self->swapchain.current_image_view,
      .imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
      .clearValue = {clear_color_black_unorm},
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

    gpu_mesh_draw(&self->triangle_mesh, current_frame->command_buffer);

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

    self->last_timestamp_nanoseconds = current_timestamp_nanoseconds;
    return true;
}
