#include "rendering/frame_data.h"

bool frame_data_init(frame_data_t* self, const render_device_t* render_device) {
    VkCommandPoolCreateInfo command_pool_create_info = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .queueFamilyIndex = render_device->graphics_queue_family_index
    };

    if (vkCreateCommandPool(render_device->vk_device, &command_pool_create_info, NULL, &self->command_pool)
        != VK_SUCCESS) {
        return false;
    }

    VkCommandBufferAllocateInfo command_buffer_allocate_info = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .commandPool = self->command_pool,
      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = 1
    };

    if (vkAllocateCommandBuffers(render_device->vk_device, &command_buffer_allocate_info, &self->command_buffer)
        != VK_SUCCESS) {
        return false;
    }

    VkFenceCreateInfo fence_create_info = {
      .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    if (vkCreateFence(render_device->vk_device, &fence_create_info, NULL, &self->render_fence) != VK_SUCCESS) {
        return false;
    }

    VkSemaphoreCreateInfo semaphore_create_info = {
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };

    if (vkCreateSemaphore(render_device->vk_device, &semaphore_create_info, NULL, &self->render_semaphore)
        != VK_SUCCESS) {
        return false;
    }

    if (vkCreateSemaphore(render_device->vk_device, &semaphore_create_info, NULL, &self->swapchain_semaphore)
        != VK_SUCCESS) {
        return false;
    }

    return true;
}

bool reset_frame_data(frame_data_t* self, const render_device_t* device) {
    if (vkResetCommandPool(device->vk_device, self->command_pool, 0) != VK_SUCCESS) {
        return false;
    }

    return true;
}

void frame_data_destroy(frame_data_t* self, const render_device_t* device) {
    vkDeviceWaitIdle(device->vk_device);

    vkDestroySemaphore(device->vk_device, self->swapchain_semaphore, NULL);
    vkDestroySemaphore(device->vk_device, self->render_semaphore, NULL);
    vkDestroyFence(device->vk_device, self->render_fence, NULL);
    vkDestroyCommandPool(device->vk_device, self->command_pool, NULL);
}
