#include "rendering/memory/gpu_allocated_buffer.h"

#include "rendering/memory/gpu_allocation.h"
#include "rendering/memory/utils.h"
#include "string.h"

bool gpu_allocated_buffer_copy_data(
  gpu_allocated_buffer_t* self, const render_device_t* device, VkDeviceSize offset, span_t source) {
    if (source.size > self->span.size - offset) {
        return false;
    }

    if (self->allocation->mapped == NULL) {
        return false;
    }

    memcpy((char*)self->allocation->mapped + offset, source.start, source.size);

    VkMappedMemoryRange mapp_memory_range = {
      .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
      .memory = self->allocation->memory,
      .offset = get_previous_offset_for_alignment(offset, self->allocation->flush_size),
      .size = get_next_offset_for_alignment(source.size, self->allocation->flush_size),
    };

    if (vkFlushMappedMemoryRanges(device->vk_device, 1, &mapp_memory_range) != VK_SUCCESS) {
        return false;
    }

    return true;
}
