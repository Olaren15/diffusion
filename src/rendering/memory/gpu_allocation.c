#include "rendering/memory/gpu_allocation.h"

#include "rendering/memory/utils.h"

bool gpu_allocation_create(
  gpu_allocation_t* self,
  const render_device_t* device,
  uint32_t memory_type_index,
  VkDeviceSize size) {

    VkMemoryAllocateInfo memory_allocate_info = {
      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .memoryTypeIndex = memory_type_index,
      .allocationSize = size,
    };

    if (
      vkAllocateMemory(device->vk_device, &memory_allocate_info, NULL, &self->memory)
      != VK_SUCCESS) {
        return false;
    }

    if (
      vkMapMemory(device->vk_device, self->memory, 0, VK_WHOLE_SIZE, 0, &self->mapped)
      != VK_SUCCESS) {
        return false;
    }

    self->allocated_size = size;
    self->current_offset = 0;
    self->flush_size = device->limits.nonCoherentAtomSize;

    return true;
}

void gpu_allocation_destroy(gpu_allocation_t* self, const render_device_t* device) {
    if (self->mapped != NULL) {
        vkUnmapMemory(device->vk_device, self->memory);
    }

    vkFreeMemory(device->vk_device, self->memory, NULL);
}

bool gpu_allocation_can_sub_allocate(
  const gpu_allocation_t* self, VkDeviceSize size, VkDeviceSize alignment) {
    VkDeviceSize next_offset = get_next_offset_for_alignment(self->current_offset, alignment);

    return next_offset + size <= self->allocated_size;
}

gpu_span_t gpu_allocation_sub_allocate(
  gpu_allocation_t* self, VkDeviceSize size, VkDeviceSize alignment) {

    VkDeviceSize new_offset = get_next_offset_for_alignment(self->current_offset, alignment);
    gpu_span_t sub_allocation = {
      .offset = new_offset,
      .size = size,
    };

    self->current_offset = new_offset + size;

    return sub_allocation;
}
