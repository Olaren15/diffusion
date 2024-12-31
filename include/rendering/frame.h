#ifndef DIFFUSION_RENDERING_FRAME_DATA_H
#define DIFFUSION_RENDERING_FRAME_DATA_H

#include "platform/vulkan.h"
#include "rendering/frame_sync.h"
#include "rendering/render_device.h"

typedef struct frame_s {
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;
    frame_sync_t sync;
} frame_t;

/**
 * Initialises a frame data for the given queue family index
 *
 * @returns true on success or false on failure
 */
bool frame_init(frame_t* self, const render_device_t* device);

/**
 * Destroy a frame data
 */
void frame_destroy(frame_t* self, const render_device_t* device);

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

#endif // DIFFUSION_RENDERING_FRAME_DATA_H
