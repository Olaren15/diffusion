#include "rendering/frame/frame_gpu_data.h"

bool frame_gpu_data_init(
  frame_gpu_data_t* self,
  const render_device_t* device,
  rebar_gpu_allocator_t* allocator,
  VkDescriptorSetLayout scene_descriptor_set_layout,
  VkDescriptorPool descriptor_pool) {
    VkBufferCreateInfo buffer_create_infos = {
      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .size = sizeof(scene_data_t),
    };

    if (!rebar_gpu_allocator_create_buffer(
          allocator, device, &buffer_create_infos, &self->uniform_buffer)) {
        return false;
    }

    VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
      .descriptorPool = descriptor_pool,
      .descriptorSetCount = 1,
      .pSetLayouts = &scene_descriptor_set_layout,
    };

    if (vkAllocateDescriptorSets(
          device->vk_device, &descriptor_set_allocate_info, &self->descriptor_set)) {
        return false;
    }

    VkDescriptorBufferInfo descriptor_buffer_info = {
      .buffer = self->uniform_buffer.vk_buffer,
      .offset = 0,
      .range = sizeof(scene_data_t),
    };

    VkWriteDescriptorSet write_descriptor_set = {
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .dstBinding = 0,
      .dstSet = self->descriptor_set,
      .descriptorCount = 1,
      .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
      .pBufferInfo = &descriptor_buffer_info,
    };

    vkUpdateDescriptorSets(device->vk_device, 1, &write_descriptor_set, 0, NULL);

    return true;
}

void frame_gpu_data_destroy(
  frame_gpu_data_t* self, const render_device_t* device, rebar_gpu_allocator_t* allocator) {
    rebar_gpu_allocator_free_buffer(allocator, device, &self->uniform_buffer);
}

bool frame_gpu_data_upload_to_gpu(frame_gpu_data_t* self, const render_device_t* device) {
    span_t data_span = {
      .start = &self->scene_data,
      .size = sizeof(scene_data_t),
    };

    if (!gpu_allocated_buffer_copy_data(&self->uniform_buffer, device, 0, data_span)) {
        return false;
    }

    return true;
}
