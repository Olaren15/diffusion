#ifndef DIFFUSION_PLATFORM_VULKAN_H
#define DIFFUSION_PLATFORM_VULKAN_H

#define VK_NO_PROTOTYPES

#include <stdbool.h>
#include <vulkan/vulkan.h>

/**
 * Load vulkan global functions
 *
 * @returns true on success or false on failure
 */
bool vulkan_load_global_functions(void);

/**
 * Load the vulkan instance functions
 */
void vulkan_load_instance_functions(VkInstance instance);

/**
 * Load the vulkan device functions
 */
void vulkan_load_device_functions(VkDevice device);

/**
 * Release the loaded global vulkan functions
 */
void vulkan_release_global_functions(void);

/**
 * Release the loaded instance-specific vulkan functions
 */
void vulkan_release_instance_functions(void);

/**
 * Release the loaded device-specific functions
 */
void vulkan_release_device_functions(void);

// Global functions
extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
extern PFN_vkCreateInstance vkCreateInstance;
extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;

// Instance functions start
extern PFN_vkDestroyInstance vkDestroyInstance;
extern PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;

extern PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerExt;
extern PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerExt;

extern PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
extern PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
extern PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
extern PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
extern PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
extern PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
extern PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
extern PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
extern PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;

extern PFN_vkCreateDevice vkCreateDevice;
extern PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
// Instance functions end

// Device functions start
extern PFN_vkDestroyDevice vkDestroyDevice;
extern PFN_vkDeviceWaitIdle vkDeviceWaitIdle;

extern PFN_vkAllocateMemory vkAllocateMemory;
extern PFN_vkFreeMemory vkFreeMemory;
extern PFN_vkMapMemory vkMapMemory;
extern PFN_vkUnmapMemory vkUnmapMemory;
extern PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
extern PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;

extern PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
extern PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
extern PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
extern PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;

extern PFN_vkCreateBuffer vkCreateBuffer;
extern PFN_vkDestroyBuffer vkDestroyBuffer;
extern PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
extern PFN_vkBindBufferMemory vkBindBufferMemory;

extern PFN_vkCreateImageView vkCreateImageView;
extern PFN_vkDestroyImageView vkDestroyImageView;

extern PFN_vkCreateCommandPool vkCreateCommandPool;
extern PFN_vkResetCommandPool vkResetCommandPool;
extern PFN_vkDestroyCommandPool vkDestroyCommandPool;
extern PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;

extern PFN_vkCreateFence vkCreateFence;
extern PFN_vkWaitForFences vkWaitForFences;
extern PFN_vkResetFences vkResetFences;
extern PFN_vkDestroyFence vkDestroyFence;
extern PFN_vkCreateSemaphore vkCreateSemaphore;
extern PFN_vkDestroySemaphore vkDestroySemaphore;

extern PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
extern PFN_vkEndCommandBuffer vkEndCommandBuffer;
extern PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2;
extern PFN_vkCmdClearColorImage vkCmdClearColorImage;
extern PFN_vkCmdBeginRendering vkCmdBeginRendering;
extern PFN_vkCmdEndRendering vkCmdEndRendering;
extern PFN_vkCmdBindPipeline vkCmdBindPipeline;
extern PFN_vkCmdSetViewport vkCmdSetViewport;
extern PFN_vkCmdSetScissor vkCmdSetScissor;
extern PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
extern PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
extern PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
extern PFN_vkCmdDraw vkCmdDraw;
extern PFN_vkCmdDrawIndexed vkCmdDrawIndexed;

extern PFN_vkGetDeviceQueue vkGetDeviceQueue;
extern PFN_vkQueueSubmit2 vkQueueSubmit2;
extern PFN_vkQueuePresentKHR vkQueuePresentKHR;

extern PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
extern PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
extern PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
extern PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
extern PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
extern PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;

extern PFN_vkCreateShaderModule vkCreateShaderModule;
extern PFN_vkDestroyShaderModule vkDestroyShaderModule;
extern PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
extern PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
extern PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
extern PFN_vkDestroyPipeline vkDestroyPipeline;
// Device functions end

#endif // DIFFUSION_PLATFORM_VULKAN_H
