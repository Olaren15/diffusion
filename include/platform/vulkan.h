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
 * Release the loaded vulkan functions
 */
void vulkan_release_functions(void);

/**
 * Get platform-specific extensions required for vulkan presentation
 *
 * @returns A dynamic array of `const char*` containing the extension names
 */
dynamic_array_t vulkan_get_required_extensions_for_presentation(void);

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
extern PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
extern PFN_vkCreateDevice vkCreateDevice;
extern PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;

// Device function
extern PFN_vkDestroyDevice vkDestroyDevice;
extern PFN_vkGetDeviceQueue vkGetDeviceQueue;

#endif // DIFFUSION_PLATFORM_VULKAN_H
