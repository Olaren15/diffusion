#include "rendering/frame/frame_gpu_data.h"

bool frame_gpu_data_init(
  frame_gpu_data_t* self, const render_device_t* device, rebar_gpu_allocator_t* allocator) {
    VkBufferCreateInfo buffer_create_infos = {
      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .size = sizeof(camera_data_t),
    };

    if (!rebar_gpu_allocator_create_buffer(
          allocator, device, &buffer_create_infos, &self->uniform)) {
        return false;
    }

    return true;
}

void frame_gpu_data_destroy(
  frame_gpu_data_t* self, const render_device_t* device, rebar_gpu_allocator_t* allocator) {
    rebar_gpu_allocator_free_buffer(allocator, device, &self->uniform);
}

bool frame_gpu_data_upload_to_gpu(frame_gpu_data_t* self, const render_device_t* device) {
    span_t data_span = {
      .start = &self->camera_data,
      .size = sizeof(camera_data_t),
    };

    if (!gpu_allocated_buffer_copy_data(&self->uniform, device, 0, data_span)) {
        return false;
    }

    return true;
}
