#ifndef DIFFUSION_RENDERING_RENDER_CONTEXT_H
#define DIFFUSION_RENDERING_RENDER_CONTEXT_H

#include "platform/vulkan.h"
#include "platform/window.h"

#include <stdbool.h>

typedef struct render_context_s {
    VkInstance vk_instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    bool validation_layers_enabled;
    VkSurfaceKHR vk_surface;
} render_context_t;

/**
 * Initialize the render context
 *
 * @param self A pointer to the new render context
 * @param window A window for which the render context needs to be compatible with. Can be NULL
 * @returns true on success or false on error
 */
bool render_context_init(render_context_t* self, const window_t* window);

/**
 * Destroy the render context and clean up
 */
void render_context_destroy(render_context_t* self);

#endif // DIFFUSION_RENDERING_RENDER_CONTEXT_H
