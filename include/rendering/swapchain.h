#ifndef DIFFUSION_RENDERING_SWAPCHAIN_H
#define DIFFUSION_RENDERING_SWAPCHAIN_H

#include "platform/vulkan.h"
#include "rendering/render_device.h"

typedef struct swapchain_s {
    VkSwapchainKHR vk_swapchain;
    VkSurfaceFormatKHR surface_format;
    VkPresentModeKHR present_mode;
    VkExtent2D extent;
    dynamic_array_t images; // VkImage
    dynamic_array_t image_views; // VkImageView
} swapchain_t;

/**
 * Initialises a swapchain
 */
bool swapchain_init(
  swapchain_t* self,
  VkSurfaceKHR surface,
  const render_device_t* device,
  const present_capabilities_t* present_capabilities,
  const window_t* window
);

/**
 * Destroy the swapchain
 */
void swapchain_destroy(swapchain_t* self, const render_device_t* device);

#endif // DIFFUSION_RENDERING_SWAPCHAIN_H
