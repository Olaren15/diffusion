#ifndef DIFFUSION_RENDERING_FRAME_SYNC_H
#define DIFFUSION_RENDERING_FRAME_SYNC_H

#include "platform/vulkan.h"
#include "rendering/render_device.h"

typedef struct frame_sync_s {
    VkSemaphore swapchain_image_available_semaphore;
    VkSemaphore render_completed_semaphore;
    VkFence render_completed_fence;
} frame_sync_t;

/**
 * Create the frame sync objects
 */
bool frame_sync_init(frame_sync_t* self, const render_device_t* device);

/**
 * Destroy the frame sync objects
 */
void frame_sync_destroy(frame_sync_t* self, const render_device_t* device);


#endif // DIFFUSION_RENDERING_FRAME_SYNC_H
