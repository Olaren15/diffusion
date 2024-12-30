#include "rendering/frame_sync.h"

bool frame_sync_init(frame_sync_t* self, const render_device_t* device) {
    VkFenceCreateInfo fence_create_info = {
      .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    if (vkCreateFence(device->vk_device, &fence_create_info, NULL, &self->render_completed_fence) != VK_SUCCESS) {
        return false;
    }

    VkSemaphoreCreateInfo semaphore_create_info = {
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };

    if (vkCreateSemaphore(device->vk_device, &semaphore_create_info, NULL, &self->render_completed_semaphore)
        != VK_SUCCESS) {
        return false;
    }

    if (vkCreateSemaphore(device->vk_device, &semaphore_create_info, NULL, &self->swapchain_image_available_semaphore)
        != VK_SUCCESS) {
        return false;
    }

    return true;
}

void frame_sync_destroy(frame_sync_t* self, const render_device_t* device) {
    vkDeviceWaitIdle(device->vk_device);

    vkDestroySemaphore(device->vk_device, self->swapchain_image_available_semaphore, NULL);
    vkDestroySemaphore(device->vk_device, self->render_completed_semaphore, NULL);
    vkDestroyFence(device->vk_device, self->render_completed_fence, NULL);
}

