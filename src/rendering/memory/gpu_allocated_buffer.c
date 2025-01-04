#include "rendering/memory/gpu_allocated_buffer.h"

#include "rendering/memory/gpu_allocation.h"
#include "rendering/memory/gpu_memory.h"
#include "string.h"

bool gpu_allocated_buffer_copy_data(
  gpu_allocated_buffer_t* self, const render_device_t* device, slice_t data) {
    size_t copy_size = data.element_count * data.element_size;
    if (copy_size > self->sub_allocation.size) {
        return false;
    }

    void* mapped_buffer;
    if (
      vkMapMemory(
        device->vk_device,
        self->sub_allocation.main_allocation->memory,
        0,
        VK_WHOLE_SIZE,
        0,
        &mapped_buffer)
      != VK_SUCCESS) {
        return false;
    }

    memcpy((char*)mapped_buffer + self->sub_allocation.offset, data.first_element, copy_size);

    VkMappedMemoryRange index_mapped_memory_range = {
      .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
      .memory = self->sub_allocation.main_allocation->memory,
      .offset = get_previous_offset_for_alignment(
        self->sub_allocation.offset, self->sub_allocation.main_allocation->flush_size),
      .size = get_next_offset_for_alignment(
        self->sub_allocation.size, self->sub_allocation.main_allocation->flush_size),
    };
    if (vkFlushMappedMemoryRanges(device->vk_device, 1, &index_mapped_memory_range) != VK_SUCCESS) {
        return false;
    }

    vkUnmapMemory(device->vk_device, self->sub_allocation.main_allocation->memory);

    return true;
}
