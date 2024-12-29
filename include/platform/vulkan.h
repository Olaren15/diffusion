#ifndef DIFFUSION_PLATFORM_VULKAN_H
#define DIFFUSION_PLATFORM_VULKAN_H

#define VK_NO_PROTOTYPES

#include "core/dynamic_array.h"

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

// Instance functions
extern PFN_vkDestroyInstance vkDestroyInstance;
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
extern PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
extern PFN_vkCreateDevice vkCreateDevice;
extern PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;

// Device function
extern PFN_vkDestroyDevice vkDestroyDevice;
extern PFN_vkGetDeviceQueue vkGetDeviceQueue;
extern PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
extern PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
extern PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
extern PFN_vkCreateImageView vkCreateImageView;
extern PFN_vkDestroyImageView vkDestroyImageView;

#endif // DIFFUSION_PLATFORM_VULKAN_H
