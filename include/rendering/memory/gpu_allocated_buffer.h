#ifndef DIFFUSION_RENDERING_MEMORY_GPU_ALLOCATED_BUFFER_H
#define DIFFUSION_RENDERING_MEMORY_GPU_ALLOCATED_BUFFER_H

#include "core/slice.h"
#include "platform/vulkan.h"
#include "rendering/memory/gpu_sub_allocation.h"
#include "rendering/render_device.h"

typedef struct gpu_allocated_buffer_s {
    VkBuffer buffer;
    gpu_sub_allocation_t sub_allocation;
} gpu_allocated_buffer_t;

bool gpu_allocated_buffer_copy_data(
  gpu_allocated_buffer_t* self, const render_device_t* device, slice_t data);

#endif // DIFFUSION_RENDERING_MEMORY_GPU_ALLOCATED_BUFFER_H
