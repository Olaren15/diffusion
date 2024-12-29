#ifndef DIFFUSION_RENDERING_DEVICE_H
#define DIFFUSION_RENDERING_DEVICE_H

#include "platform/vulkan.h"
#include "rendering/render_context.h"

typedef struct present_capabilities_s {
    VkSurfaceCapabilitiesKHR vk_surface_capabilities;
    dynamic_array_t supported_surface_formats; // VkSurfaceFormatKHR
    dynamic_array_t supported_present_modes; // VkPresentModeKHR
} present_capabilities_t;

typedef struct render_device {
    VkDevice vk_device;
    VkQueue graphics_queue;
    uint32_t graphics_queue_family_index;
    VkQueue present_queue;
    uint32_t present_queue_family_index;
    present_capabilities_t present_capabilities;
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
