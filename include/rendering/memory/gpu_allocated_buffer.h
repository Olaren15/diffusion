﻿#ifndef DIFFUSION_RENDERING_MEMORY_GPU_ALLOCATED_BUFFER_H
#define DIFFUSION_RENDERING_MEMORY_GPU_ALLOCATED_BUFFER_H

#include "core/memory/span.h"
#include "platform/vulkan.h"
#include "rendering/memory/gpu_allocation.h"
#include "rendering/memory/gpu_span.h"
#include "rendering/render_device.h"

typedef struct gpu_allocated_buffer_s {
    VkBuffer vk_buffer;
    gpu_span_t span;
    gpu_allocation_t* allocation;
} gpu_allocated_buffer_t;

bool gpu_allocated_buffer_copy_data(
  gpu_allocated_buffer_t* self, const render_device_t* device, VkDeviceSize offset, span_t source);

#endif // DIFFUSION_RENDERING_MEMORY_GPU_ALLOCATED_BUFFER_H
