#include "engine.h"

#include "project.h"

const uint32_t DEFAULT_WIDTH = 1920;
const uint32_t DEFAULT_HEIGHT = 1080;
const uint32_t MAX_FRAME_WAIT_TIME = 10000000; // 1 second

/*
 * Do one iteration (frame) worth of stuff
 *
 * @returns Whether the app should continue running
 */
static bool engine_iterate(engine_t* self);

bool engine_init(engine_t* self) {
    if (!window_create(PROJECT_NAME, DEFAULT_WIDTH, DEFAULT_HEIGHT, &self->window)) {
        return false;
    }

    if (!render_context_init(&self->render_context, &self->window)) {
        return false;
    }

    if (!render_device_init(&self->render_device, &self->render_context)) {
        return false;
    }

    if (!swapchain_init(
          &self->swapchain,
          self->render_context.vk_surface,
          &self->render_device,
          &self->render_device.present_capabilities,
          &self->window
        )) {
        return false;
    }

    self->current_frame = 0;
    for (size_t i = 0; i < ENGINE_MAX_FRAMES_IN_FLIGHT; i++) {
        frame_data_init(self->frames_data + i, &self->render_device);
    }

    return true;
}

void engine_destroy(engine_t* self) {
    for (size_t i = 0; i < ENGINE_MAX_FRAMES_IN_FLIGHT; i++) {
        frame_data_destroy(self->frames_data + i, &self->render_device);
    }

    swapchain_destroy(&self->swapchain, &self->render_device);
    render_device_destroy(&self->render_device);
    render_context_destroy(&self->render_context);
    window_destroy(&self->window);
}

void engine_run(engine_t* self) {
    bool continue_running = true;
    event_t event;

    while (continue_running) {
        while (continue_running && window_poll_event(&self->window, &event)) {
            continue_running &= event.type != EVENT_TYPE_QUIT;
        }

        if (continue_running) {
            continue_running &= engine_iterate(self);
        }
    }
}

static bool engine_iterate(engine_t* self) {
    self->current_frame = (self->current_frame + 1) % ENGINE_MAX_FRAMES_IN_FLIGHT;
    frame_data_t* current_frame_data = self->frames_data + self->current_frame;

    if (vkWaitForFences(self->render_device.vk_device, 1, &current_frame_data->render_fence, true, MAX_FRAME_WAIT_TIME)
        != VK_SUCCESS) {
        return false;
    }
    if (vkResetFences(self->render_device.vk_device, 1, &current_frame_data->render_fence) != VK_SUCCESS) {
        return false;
    }

    uint32_t swapchain_image_index;
    if (vkAcquireNextImageKHR(
          self->render_device.vk_device,
          self->swapchain.vk_swapchain,
          MAX_FRAME_WAIT_TIME,
          current_frame_data->swapchain_semaphore,
          NULL,
          &swapchain_image_index
        )
        != VK_SUCCESS) {
        return false;
    }
    VkImage current_image = ((VkImage*)self->swapchain.images.data)[swapchain_image_index];

    reset_frame_data(current_frame_data, &self->render_device);

    VkCommandBufferBeginInfo command_buffer_begin_info = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };

    if (vkBeginCommandBuffer(current_frame_data->command_buffer, &command_buffer_begin_info) != VK_SUCCESS) {
        return false;
    }

    VkImageMemoryBarrier2 write_to_swapchain_image_barrier = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
      .srcStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .newLayout = VK_IMAGE_LAYOUT_GENERAL,
      .image = current_image,
      .subresourceRange = {
                           .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                           .baseMipLevel = 0,
                           .levelCount = VK_REMAINING_MIP_LEVELS,
                           .baseArrayLayer = 0,
                           .layerCount = VK_REMAINING_MIP_LEVELS,
                           }
    };

    VkDependencyInfo write_to_swapchain_image_dependency_info = {
      .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
      .imageMemoryBarrierCount = 1,
      .pImageMemoryBarriers = &write_to_swapchain_image_barrier
    };

    vkCmdPipelineBarrier2(current_frame_data->command_buffer, &write_to_swapchain_image_dependency_info);

    VkClearColorValue clear_color_value = {
      .float32 = {1.0f, 1.0f, 1.0f, 1.0f}
    };

    VkImageSubresourceRange clear_range = {
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .baseMipLevel = 0,
      .levelCount = VK_REMAINING_MIP_LEVELS,
      .baseArrayLayer = 0,
      .layerCount = VK_REMAINING_MIP_LEVELS,
    };

    vkCmdClearColorImage(
      current_frame_data->command_buffer, current_image, VK_IMAGE_LAYOUT_GENERAL, &clear_color_value, 1, &clear_range
    );

    VkImageMemoryBarrier2 present_swapchain_image_barrier = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
      .srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
      .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .oldLayout = VK_IMAGE_LAYOUT_GENERAL,
      .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
      .image = current_image,
      .subresourceRange = {
                           .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                           .baseMipLevel = 0,
                           .levelCount = VK_REMAINING_MIP_LEVELS,
                           .baseArrayLayer = 0,
                           .layerCount = VK_REMAINING_MIP_LEVELS,
                           }
    };

    VkDependencyInfo present_swapchain_image_dependency_info = {
      .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
      .imageMemoryBarrierCount = 1,
      .pImageMemoryBarriers = &present_swapchain_image_barrier
    };

    vkCmdPipelineBarrier2(current_frame_data->command_buffer, &present_swapchain_image_dependency_info);

    if (vkEndCommandBuffer(current_frame_data->command_buffer) != VK_SUCCESS) {
        return false;
    }

    VkSemaphoreSubmitInfo wait_swapchain_semaphore_submit_info = {
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
      .semaphore = current_frame_data->swapchain_semaphore,
      .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
      .value = 1,
    };

    VkSemaphoreSubmitInfo signal_render_semaphore_submit_info = {
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
      .semaphore = current_frame_data->render_semaphore,
      .stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT,
      .value = 1
    };

    VkCommandBufferSubmitInfo command_buffer_submit_info = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
      .commandBuffer = current_frame_data->command_buffer,
    };

    VkSubmitInfo2 submit_info = {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
      .waitSemaphoreInfoCount = 1,
      .pWaitSemaphoreInfos = &wait_swapchain_semaphore_submit_info,
      .signalSemaphoreInfoCount = 1,
      .pSignalSemaphoreInfos = &signal_render_semaphore_submit_info,
      .commandBufferInfoCount = 1,
      .pCommandBufferInfos = &command_buffer_submit_info,
    };

    if (vkQueueSubmit2(self->render_device.graphics_queue, 1, &submit_info, current_frame_data->render_fence)
        != VK_SUCCESS) {
        return false;
    }

    VkPresentInfoKHR present_info = {
      .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
      .swapchainCount = 1,
      .pSwapchains = &self->swapchain.vk_swapchain,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = &current_frame_data->render_semaphore,
      .pImageIndices = &swapchain_image_index,
    };

    if (vkQueuePresentKHR(self->render_device.present_queue, &present_info) != VK_SUCCESS) {
        return false;
    }

    return true;
}
