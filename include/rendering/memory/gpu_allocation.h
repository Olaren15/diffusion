#ifndef DIFFUSION_RENDERING_MEMORY_GPU_ALLOCATION_H
#define DIFFUSION_RENDERING_MEMORY_GPU_ALLOCATION_H

#include "platform/vulkan.h"
#include "rendering/memory/gpu_sub_allocation.h"
#include "rendering/render_device.h"

typedef struct gpu_allocation_s {
    VkDeviceMemory memory;
    VkDeviceSize allocated_size;
    VkDeviceSize current_offset;
    VkDeviceSize flush_size;
} gpu_allocation_t;

bool gpu_allocation_create(
  gpu_allocation_t* self,
  const render_device_t* device,
  uint32_t memory_type_index,
  VkDeviceSize size);

void gpu_allocation_destroy(gpu_allocation_t* self, const render_device_t* device);

bool gpu_allocation_can_sub_allocate(
  const gpu_allocation_t* self, VkDeviceSize size, VkDeviceSize alignment);

gpu_sub_allocation_t gpu_allocation_sub_allocate(
  gpu_allocation_t* self, VkDeviceSize size, VkDeviceSize alignment);

#endif // DIFFUSION_RENDERING_MEMORY_GPU_ALLOCATION_H
