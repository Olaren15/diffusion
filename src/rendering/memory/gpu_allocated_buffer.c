#include "rendering/memory/gpu_allocated_buffer.h"

#include "rendering/memory/gpu_allocation.h"
#include "rendering/memory/utils.h"
#include "string.h"

bool gpu_allocated_buffer_copy_data(
  gpu_allocated_buffer_t* self, const render_device_t* device, VkDeviceSize offset, span_t source) {
    if (source.size > self->span.size - offset) {
        return false;
    }

    gpu_allocation_t* gpu_allocation = ((gpu_allocation_t*)follow_dynamic_array_reference(
      self->allocation_reference));

    if (gpu_allocation->mapped == NULL) {
        return false;
    }

    memcpy((char*)gpu_allocation->mapped + self->span.offset + offset, source.start, source.size);

    VkMappedMemoryRange mapp_memory_range = {
      .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
      .memory = gpu_allocation->memory,
      .offset = get_previous_offset_for_alignment(offset, gpu_allocation->flush_size),
      .size = get_next_offset_for_alignment(source.size, gpu_allocation->flush_size),
    };

    if (vkFlushMappedMemoryRanges(device->vk_device, 1, &mapp_memory_range) != VK_SUCCESS) {
        return false;
    }

    return true;
}
