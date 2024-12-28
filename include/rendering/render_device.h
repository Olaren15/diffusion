#ifndef DIFFUSION_RENDERING_DEVICE_H
#define DIFFUSION_RENDERING_DEVICE_H

#include "platform/vulkan.h"
#include "rendering/render_context.h"

typedef struct render_device {
    VkDevice device;
    VkQueue graphics_queue;
    VkQueue present_queue;
} render_device_t;

/**
 * Picks a vulkan device and creates resources associated with it
 *
 * @returns true on success or false on failure
 */
bool render_device_init(render_device_t* self, const render_context_t* render_context);

/**
 * Clean up the device
 */
void render_device_destroy(render_device_t* self);

#endif // DIFFUSION_RENDERING_DEVICE_H
