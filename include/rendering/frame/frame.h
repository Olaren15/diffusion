#ifndef DIFFUSION_RENDERING_FRAME_DATA_H
#define DIFFUSION_RENDERING_FRAME_DATA_H

#include "platform/vulkan.h"
#include "rendering/frame/frame_gpu_data.h"
#include "rendering/frame/frame_sync.h"
#include "rendering/memory/gpu_allocated_buffer.h"
#include "rendering/memory/rebar_gpu_allocator.h"
#include "rendering/render_device.h"

typedef struct frame_s {
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;
    frame_sync_t sync;
    frame_gpu_data_t gpu_data;
} frame_t;

/**
 * Initialises a frame data for the given queue family index
 *
 * @returns true on success or false on failure
 */
bool frame_init(frame_t* self, const render_device_t* device, rebar_gpu_allocator_t* allocator);

/**
 * Destroy a frame data
 */
void frame_destroy(frame_t* self, const render_device_t* device, rebar_gpu_allocator_t* allocator);

/**
 * Wait until the frame is done rendering
 */
bool frame_wait_for_render_completed(
  frame_t* self, const render_device_t* device, uint32_t timeout);

/**
 * Begin recording a new frame
 */
bool frame_begin_new(frame_t* self, const render_device_t* device);

/**
 * End a frame and submit it for execution
 */
bool frame_end(frame_t* self, const render_device_t* device);

/**
 * Updates the gpu data for this frame
 */
bool frame_update_gpu_data(frame_t* self, const render_device_t* device, const camera_data_t* camera_data);

#endif // DIFFUSION_RENDERING_FRAME_DATA_H
