#include "rendering/frame/frame.h"

bool frame_init(
  frame_t* self,
  const render_device_t* device,
  rebar_gpu_allocator_t* allocator,
  VkDescriptorSetLayout scene_descriptor_set_layout,
  VkDescriptorPool descriptor_pool) {
    VkCommandPoolCreateInfo command_pool_create_info = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .queueFamilyIndex = device->graphics_queue_family_index,
    };

    if (
      vkCreateCommandPool(device->vk_device, &command_pool_create_info, NULL, &self->command_pool)
      != VK_SUCCESS) {
        return false;
    }

    VkCommandBufferAllocateInfo command_buffer_allocate_info = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .commandPool = self->command_pool,
      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = 1,
    };

    if (
      vkAllocateCommandBuffers(
        device->vk_device, &command_buffer_allocate_info, &self->command_buffer)
      != VK_SUCCESS) {
        return false;
    }

    if (!frame_sync_init(&self->sync, device)) {
        return false;
    }

    if (!frame_gpu_data_init(
          &self->gpu_data, device, allocator, scene_descriptor_set_layout, descriptor_pool)) {
        return false;
    }

    return true;
}

void frame_destroy(frame_t* self, const render_device_t* device, rebar_gpu_allocator_t* allocator) {
    frame_gpu_data_destroy(&self->gpu_data, device, allocator);
    frame_sync_destroy(&self->sync, device);
    vkDestroyCommandPool(device->vk_device, self->command_pool, NULL);
}

bool frame_wait_for_render_completed(
  frame_t* self, const render_device_t* device, uint32_t timeout) {
    if (
      vkWaitForFences(device->vk_device, 1, &self->sync.render_completed_fence, true, timeout)
      != VK_SUCCESS) {
        return false;
    }

    return true;
}

bool frame_begin_new(frame_t* self, const render_device_t* device) {
    if (vkResetFences(device->vk_device, 1, &self->sync.render_completed_fence) != VK_SUCCESS) {
        return false;
    }

    if (vkResetCommandPool(device->vk_device, self->command_pool, 0) != VK_SUCCESS) {
        return false;
    }

    VkCommandBufferBeginInfo command_buffer_begin_info = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };

    if (vkBeginCommandBuffer(self->command_buffer, &command_buffer_begin_info) != VK_SUCCESS) {
        return false;
    }

    return true;
}

bool frame_end(frame_t* self, const render_device_t* device) {
    if (vkEndCommandBuffer(self->command_buffer) != VK_SUCCESS) {
        return false;
    }

    VkSemaphoreSubmitInfo swapchain_image_available_submit_info = {
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
      .semaphore = self->sync.swapchain_image_available_semaphore,
      .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
      .value = 1,
    };

    VkSemaphoreSubmitInfo render_completed_submit_info = {
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
      .semaphore = self->sync.render_completed_semaphore,
      .stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT,
      .value = 1,
    };

    VkCommandBufferSubmitInfo command_buffer_submit_info = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
      .commandBuffer = self->command_buffer,
    };

    VkSubmitInfo2 submit_info = {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
      .waitSemaphoreInfoCount = 1,
      .pWaitSemaphoreInfos = &swapchain_image_available_submit_info,
      .signalSemaphoreInfoCount = 1,
      .pSignalSemaphoreInfos = &render_completed_submit_info,
      .commandBufferInfoCount = 1,
      .pCommandBufferInfos = &command_buffer_submit_info,
    };

    if (
      vkQueueSubmit2(device->graphics_queue, 1, &submit_info, self->sync.render_completed_fence)
      != VK_SUCCESS) {
        return false;
    }

    return true;
}

bool frame_update_gpu_data(
  frame_t* self, const render_device_t* device, const scene_data_t* scene_data) {
    self->gpu_data.scene_data = *scene_data;

    if (!frame_gpu_data_upload_to_gpu(&self->gpu_data, device)) {
        return false;
    }

    return true;
}
