#ifndef DIFFUSION_RENDERING_MEMORY_GPU_SUB_ALLOCATION_H
#define DIFFUSION_RENDERING_MEMORY_GPU_SUB_ALLOCATION_H

#include "platform/vulkan.h"

typedef struct gpu_sub_allocation_s {
    const struct gpu_allocation_s* main_allocation;
    VkDeviceSize offset;
    VkDeviceSize size;
} gpu_sub_allocation_t;

#endif // DIFFUSION_RENDERING_MEMORY_GPU_SUB_ALLOCATION_H
