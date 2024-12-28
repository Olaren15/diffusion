#ifndef DIFFUSION_RENDERING_DEVICE_H
#define DIFFUSION_RENDERING_DEVICE_H

#include "platform/vulkan.h"

typedef struct device_s {
    VkDevice device;
    VkQueue graphics_queue;
} device_t;

/**
 * Picks a vulkan device and creates resources associated with it
 *
 * @returns true on success or false on failure
 */
bool device_init(device_t* self, VkInstance instance);

/**
 * Clean up the device
 */
void device_destroy(device_t* self);

#endif // DIFFUSION_RENDERING_DEVICE_H
