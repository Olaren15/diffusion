#include "rendering/swapchain.h"

#include "core/math.h"
#include "rendering/vk_utils.h"

bool swapchain_init_impl(
  swapchain_t* self,
  const render_context_t* context,
  const render_device_t* device,
  VkSwapchainKHR previous_swapchain);
bool choose_surface_format(
  const dynamic_array_t* available_formats, VkSurfaceFormatKHR* chosen_format);
bool choose_presentation_mode(
  const dynamic_array_t* available_modes, VkPresentModeKHR* chosen_mode);
void destroy_swapchain_resources(swapchain_t* self, const render_device_t* device);

bool swapchain_init(
  swapchain_t* self, const render_context_t* context, const render_device_t* device) {
    return swapchain_init_impl(self, context, device, VK_NULL_HANDLE);
}

bool swapchain_recreate(
  swapchain_t* self, const render_context_t* context, const render_device_t* device) {
    VkSwapchainKHR old_swapchain = self->vk_swapchain;

    destroy_swapchain_resources(self, device);

    if (!swapchain_init_impl(self, context, device, old_swapchain)) {
        return false;
    }

    vkDestroySwapchainKHR(device->vk_device, old_swapchain, NULL);

    return true;
}

void swapchain_destroy(swapchain_t* self, const render_device_t* device) {
    destroy_swapchain_resources(self, device);

    vkDestroySwapchainKHR(device->vk_device, self->vk_swapchain, NULL);
}

bool swapchain_init_impl(
  swapchain_t* self,
  const render_context_t* context,
  const render_device_t* device,
  VkSwapchainKHR previous_swapchain) {
    if (!choose_surface_format(
          &device->present_capabilities.supported_surface_formats, &self->surface_format)) {
        return false;
    }

    if (!choose_presentation_mode(
          &device->present_capabilities.supported_present_modes, &self->present_mode)) {
        return false;
    }

    self->extent.width = clamp_uint32_t(
      device->present_capabilities.vk_surface_capabilities.minImageExtent.width,
      device->present_capabilities.vk_surface_capabilities.maxImageExtent.width,
      device->present_capabilities.vk_surface_capabilities.currentExtent.width);
    self->extent.height = clamp_uint32_t(
      device->present_capabilities.vk_surface_capabilities.minImageExtent.height,
      device->present_capabilities.vk_surface_capabilities.maxImageExtent.height,
      device->present_capabilities.vk_surface_capabilities.currentExtent.height);

    uint32_t image_count = device->present_capabilities.vk_surface_capabilities.minImageCount + 1;
    if (
      device->present_capabilities.vk_surface_capabilities.maxImageCount > 0
      && image_count > device->present_capabilities.vk_surface_capabilities.maxImageCount) {
        image_count = device->present_capabilities.vk_surface_capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapchain_create_infos = {
      .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
      .surface = context->vk_surface,
      .minImageCount = image_count,
      .imageFormat = self->surface_format.format,
      .imageColorSpace = self->surface_format.colorSpace,
      .imageExtent = self->extent,
      .imageArrayLayers = 1,
      .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
      .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .preTransform = device->present_capabilities.vk_surface_capabilities.currentTransform,
      .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
      .presentMode = self->present_mode,
      .clipped = VK_TRUE,
      .oldSwapchain = previous_swapchain,
    };

    if (
      vkCreateSwapchainKHR(device->vk_device, &swapchain_create_infos, NULL, &self->vk_swapchain)
      != VK_SUCCESS) {
        return false;
    }

    vkGetSwapchainImagesKHR(device->vk_device, self->vk_swapchain, &image_count, NULL);
    self->images = dynamic_array_allocate_size(sizeof(VkImage), image_count);
    self->image_views = dynamic_array_allocate_size(sizeof(VkImageView), image_count);
    vkGetSwapchainImagesKHR(device->vk_device, self->vk_swapchain, &image_count, self->images.data);

    for (size_t i = 0; i < self->images.element_count; i++) {
        VkImage image = ((VkImage*)self->images.data)[i];
        VkImageView* image_view = ((VkImageView*)self->image_views.data) + i;

        VkImageViewCreateInfo image_view_create_info = {
          .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
          .image = image,
          .viewType = VK_IMAGE_VIEW_TYPE_2D,
          .format = self->surface_format.format,
          .components =
            {.r = VK_COMPONENT_SWIZZLE_IDENTITY,
                         .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                         .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                         .a = VK_COMPONENT_SWIZZLE_IDENTITY},
          .subresourceRange = subresource_range_color_all_mips,
        };

        if (
          vkCreateImageView(device->vk_device, &image_view_create_info, NULL, image_view)
          != VK_SUCCESS) {
            return false;
        }
    }

    self->suboptimal = false;
    self->out_of_date = false;

    return true;
}

bool choose_surface_format(
  const dynamic_array_t* available_formats, VkSurfaceFormatKHR* chosen_format) {
    if (available_formats->element_count == 0) {
        return false;
    }

    static VkSurfaceFormatKHR desired_formats[] = {
      {.format = VK_FORMAT_A2B10G10R10_UNORM_PACK32,
       .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR                                              },
      {.format = VK_FORMAT_B8G8R8A8_UNORM,           .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR},
      {.format = VK_FORMAT_B8G8R8A8_SRGB,            .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}
    };

    for (size_t i = 0; i < 3; i++) {
        VkSurfaceFormatKHR desired_format = desired_formats[i];

        for (size_t j = 0; j < available_formats->element_count; j++) {
            VkSurfaceFormatKHR available_format = ((VkSurfaceFormatKHR*)available_formats->data)[j];
            if (
              desired_format.format == available_format.format
              && desired_format.colorSpace == available_format.colorSpace) {
                *chosen_format = available_format;
                return true;
            }
        }
    }

    *chosen_format = ((VkSurfaceFormatKHR*)available_formats->data)[0];
    return true;
}

bool choose_presentation_mode(
  const dynamic_array_t* available_modes, VkPresentModeKHR* chosen_mode) {
    if (available_modes->element_count == 0) {
        return false;
    }

    static VkPresentModeKHR desired_present_modes[] = {
      VK_PRESENT_MODE_MAILBOX_KHR,
      VK_PRESENT_MODE_FIFO_RELAXED_KHR,
      VK_PRESENT_MODE_FIFO_KHR,
    };

    for (size_t i = 0; i < 3; i++) {
        VkPresentModeKHR desired_mode = desired_present_modes[i];

        for (size_t j = 0; j < available_modes->element_count; j++) {
            VkPresentModeKHR available_mode = ((VkPresentModeKHR*)available_modes->data)[j];

            if (desired_mode == available_mode) {
                *chosen_mode = available_mode;
                return true;
            }
        }
    }

    *chosen_mode = ((VkPresentModeKHR*)available_modes->data)[0];
    return true;
}

void destroy_swapchain_resources(swapchain_t* self, const render_device_t* device) {
    vkDeviceWaitIdle(device->vk_device);

    for (size_t i = 0; i < self->image_views.element_count; i++) {
        VkImageView image_view = ((VkImageView*)self->image_views.data)[i];
        vkDestroyImageView(device->vk_device, image_view, NULL);
        image_view = VK_NULL_HANDLE;
    }

    dynamic_array_free(&self->image_views);
    dynamic_array_free(&self->images);

    self->surface_format = (VkSurfaceFormatKHR){.format = 0, .colorSpace = 0};
    self->present_mode = 0;
    self->extent = (VkExtent2D){.width = 0, .height = 0};
    self->suboptimal = false;
    self->out_of_date = false;
}

bool swapchain_acquire_next_image(
  swapchain_t* self, const render_device_t* device, uint32_t wait_time, const frame_t* frame) {
    VkResult result = vkAcquireNextImageKHR(
      device->vk_device,
      self->vk_swapchain,
      wait_time,
      frame->sync.swapchain_image_available_semaphore,
      VK_NULL_HANDLE,
      &self->current_image_index);

    switch (result) {
        case VK_SUCCESS:
            break;
        case VK_SUBOPTIMAL_KHR:
            self->suboptimal = true;
            break;
        case VK_ERROR_OUT_OF_DATE_KHR:
            self->out_of_date = true;
            break;
        default:
            return false;
    }

    self->current_image = ((VkImage*)self->images.data)[self->current_image_index];
    self->current_image_view = ((VkImageView*)self->image_views.data)[self->current_image_index];

    return true;
}

void swapchain_prepare_current_image_for_writing(
  const swapchain_t* self, VkCommandBuffer command_buffer) {
    VkImageMemoryBarrier2 write_to_swapchain_image_barrier = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
      .srcStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      .image = self->current_image,
      .subresourceRange = subresource_range_color_all_mips,
    };

    VkDependencyInfo write_to_swapchain_image_dependency_info = {
      .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
      .imageMemoryBarrierCount = 1,
      .pImageMemoryBarriers = &write_to_swapchain_image_barrier};

    vkCmdPipelineBarrier2(command_buffer, &write_to_swapchain_image_dependency_info);
}

void swapchain_prepare_current_image_for_presentation(
  const swapchain_t* self, VkCommandBuffer command_buffer) {
    VkImageMemoryBarrier2 present_swapchain_image_barrier = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
      .srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
      .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
      .image = self->current_image,
      .subresourceRange = subresource_range_color_all_mips,
    };

    VkDependencyInfo present_swapchain_image_dependency_info = {
      .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
      .imageMemoryBarrierCount = 1,
      .pImageMemoryBarriers = &present_swapchain_image_barrier};

    vkCmdPipelineBarrier2(command_buffer, &present_swapchain_image_dependency_info);
}

bool swapchain_present(swapchain_t* self, const render_device_t* device, const frame_t* frame) {
    VkPresentInfoKHR present_info = {
      .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
      .swapchainCount = 1,
      .pSwapchains = &self->vk_swapchain,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = &frame->sync.render_completed_semaphore,
      .pImageIndices = &self->current_image_index,
    };

    VkResult result = vkQueuePresentKHR(device->graphics_queue, &present_info);
    switch (result) {
        case VK_SUCCESS:
            break;
        case VK_SUBOPTIMAL_KHR:
            self->suboptimal = true;
            break;
        case VK_ERROR_OUT_OF_DATE_KHR:
            self->out_of_date = true;
            break;
        default:
            return false;
    }

    return true;
}
