#include "rendering/memory/gpu_memory.h"

VkDeviceSize get_next_offset_for_alignment(VkDeviceSize offset, VkDeviceSize alignment) {
    VkDeviceSize bytes_out_of_alignment = offset % alignment;

    if (bytes_out_of_alignment == 0) {
        return offset;
    }

    VkDeviceSize bytes_to_reach_alignment = alignment - bytes_out_of_alignment;
    return offset + bytes_to_reach_alignment;
}

VkDeviceSize get_previous_offset_for_alignment(VkDeviceSize offset, VkDeviceSize alignment) {
    VkDeviceSize bytes_out_of_alignment = offset % alignment;

    if (bytes_out_of_alignment == 0) {
        return offset;
    }

    return offset - bytes_out_of_alignment;
}
