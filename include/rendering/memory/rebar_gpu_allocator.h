#ifndef DIFFUSION_RENDERING_MEMORY_H
#define DIFFUSION_RENDERING_MEMORY_H

#include "platform/vulkan.h"
#include "rendering/memory/gpu_allocated_buffer.h"
#include "rendering/memory/gpu_allocation.h"
#include "rendering/render_device.h"

typedef struct rebar_gpu_allocator_s {
    dynamic_array_t allocations; // gpu_allocation_t
    VkMemoryType rebar_memory_type;
    uint32_t memory_type_index;
} rebar_gpu_allocator_t;

/**
 * Checks if the resizable bar gpu allocator supports a specific device.
 *
 * @returns true if the device is supported, false if the device is unsupported
 */
bool rebar_gpu_allocator_available(const render_device_t* device);

/**
 * Initialises the resizable bar allocator for use with a specific device.
 *
 * @returns true on success, false on failure
 */
bool rebar_gpu_allocator_init(rebar_gpu_allocator_t* self, const render_device_t* device);

/**
 * Destroys the allocator and frees all the memory associated with it.
 * Make sure to destroy evey resource that could use this memory.
 */
void rebar_gpu_allocator_destroy(rebar_gpu_allocator_t* self, const render_device_t* device);

bool rebar_gpu_allocator_create_buffer(
  rebar_gpu_allocator_t* self,
  const render_device_t* device,
  const VkBufferCreateInfo* buffer_create_info,
  gpu_allocated_buffer_t* buffer);

bool rebar_gpu_allocator_free_buffer(
  rebar_gpu_allocator_t* self, const render_device_t* device, gpu_allocated_buffer_t* buffer);

#endif // DIFFUSION_RENDERING_MEMORY_H
