﻿#ifndef DIFFUSION_RENDERING_SWAPCHAIN_H
#define DIFFUSION_RENDERING_SWAPCHAIN_H

#include "platform/vulkan.h"
#include "rendering/frame/frame.h"
#include "rendering/render_context.h"
#include "rendering/render_device.h"

typedef struct swapchain_s {
    VkSwapchainKHR vk_swapchain;
    VkSurfaceFormatKHR surface_format;
    VkPresentModeKHR present_mode;
    VkExtent2D extent;
    dynamic_array_t images; // VkImage
    dynamic_array_t image_views; // VkImageView
    uint32_t current_image_index;
    VkImage current_image;
    VkImageView current_image_view;
    bool suboptimal;
    bool out_of_date;
} swapchain_t;

/**
 * Initialises a swapchain
 */
bool swapchain_init(
  swapchain_t* self, const render_context_t* context, const render_device_t* device);

/**
 * Recreates the swapchain
 */
bool swapchain_recreate(
  swapchain_t* self, const render_context_t* context, const render_device_t* device);

/**
 * Destroy the swapchain
 */
void swapchain_destroy(swapchain_t* self, const render_device_t* device);

/**
 * Acquire the next swapchain image
 */
bool swapchain_acquire_next_image(
  swapchain_t* self, const render_device_t* device, uint32_t wait_time, const frame_t* frame);

/**
 * Transition the current swapchain image to a format suitable for writing
 */
void swapchain_prepare_current_image_for_writing(
  const swapchain_t* self, VkCommandBuffer command_buffer);

/**
 * Transition the current swapchain image to a format suitable for presentation
 */
void swapchain_prepare_current_image_for_presentation(
  const swapchain_t* self, VkCommandBuffer command_buffer);

/**
 * Submit the current swapchain image for presentation
 */
bool swapchain_present(swapchain_t* self, const render_device_t* device, const frame_t* frame);

#endif // DIFFUSION_RENDERING_SWAPCHAIN_H
