#ifndef DIFFUSION_RENDERING_FRAME_DATA_H
#define DIFFUSION_RENDERING_FRAME_DATA_H

#include "platform/vulkan.h"
#include "rendering/render_device.h"

typedef struct frame_data_s {
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;
    VkSemaphore swapchain_semaphore;
    VkSemaphore render_semaphore;
    VkFence render_fence;
} frame_data_t;

/**
 * Initialises a frame data for the given queue family index
 *
 * @returns true on success or false on failure
 */
bool frame_data_init(frame_data_t* self, const render_device_t* device);

/**
 * Resets the frame data to be used for a new frame
 */
bool reset_frame_data(frame_data_t* self, const render_device_t* device);

/**
 * Destroy a frame data
 */
void frame_data_destroy(frame_data_t* self, const render_device_t* device);

#endif // DIFFUSION_RENDERING_FRAME_DATA_H
