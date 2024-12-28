#ifndef DIFFUSION_RENDERING_RENDERER_H
#define DIFFUSION_RENDERING_RENDERER_H

#include "platform/vulkan.h"
#include "rendering/device.h"

#include <stdbool.h>

typedef struct renderer_s {
    VkInstance vk_instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    bool validation_layers_enabled;
    device_t device;
} renderer_t;

/**
 * Initialize the renderer
 *
 * @returns true on success or false on error
 */

bool renderer_init(renderer_t* self);

/**
 * Destroy the renderer and clean up
 */
void renderer_destroy(renderer_t* self);

/**
 * Render one frame
 */
void renderer_render(const renderer_t* self);

#endif // DIFFUSION_RENDERING_RENDERER_H
