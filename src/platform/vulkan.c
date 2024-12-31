#include "platform/vulkan.h"

#ifdef WIN32
#include "platform/win32/win32_types.h"

#include <vulkan/vulkan_win32.h>
#endif

static void* loaded_library = NULL;

bool vulkan_load_global_functions(void) {
#ifdef WIN32
    HMODULE library = LoadLibraryW(L"vulkan-1.dll");
    if (library == NULL) {
        return false;
    }

    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)(void (*)(void))GetProcAddress(
      library, "vkGetInstanceProcAddr");
    loaded_library = library;
#endif

    if (vkGetInstanceProcAddr == NULL) {
        return false;
    }

    vkCreateInstance = (PFN_vkCreateInstance)vkGetInstanceProcAddr(NULL, "vkCreateInstance");
    vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)
      vkGetInstanceProcAddr(NULL, "vkEnumerateInstanceLayerProperties");

    return true;
}

void vulkan_load_instance_functions(VkInstance instance) {
    vkDestroyInstance = (PFN_vkDestroyInstance)vkGetInstanceProcAddr(instance, "vkDestroyInstance");
    vkCreateDebugUtilsMessengerExt = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
    vkDestroyDebugUtilsMessengerExt = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
    vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)vkGetInstanceProcAddr(
      instance, "vkEnumeratePhysicalDevices");
    vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)vkGetInstanceProcAddr(
      instance, "vkGetPhysicalDeviceProperties");
    vkEnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)
      vkGetInstanceProcAddr(instance, "vkEnumerateDeviceExtensionProperties");
    vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)
      vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties");
    vkGetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)
      vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
      vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
    vkGetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)
      vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
    vkGetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)
      vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
    vkDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)vkGetInstanceProcAddr(
      instance, "vkDestroySurfaceKHR");
    vkCreateDevice = (PFN_vkCreateDevice)vkGetInstanceProcAddr(instance, "vkCreateDevice");
    vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)vkGetInstanceProcAddr(
      instance, "vkGetDeviceProcAddr");
}

void vulkan_load_device_functions(VkDevice device) {
    vkDestroyDevice = (PFN_vkDestroyDevice)vkGetDeviceProcAddr(device, "vkDestroyDevice");
    vkDeviceWaitIdle = (PFN_vkDeviceWaitIdle)vkGetDeviceProcAddr(device, "vkDeviceWaitIdle");
    vkGetDeviceQueue = (PFN_vkGetDeviceQueue)vkGetDeviceProcAddr(device, "vkGetDeviceQueue");
    vkCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)vkGetDeviceProcAddr(
      device, "vkCreateSwapchainKHR");
    vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)vkGetDeviceProcAddr(
      device, "vkGetSwapchainImagesKHR");
    vkAcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)vkGetDeviceProcAddr(
      device, "vkAcquireNextImageKHR");
    vkDestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)vkGetDeviceProcAddr(
      device, "vkDestroySwapchainKHR");
    vkCreateImageView = (PFN_vkCreateImageView)vkGetDeviceProcAddr(device, "vkCreateImageView");
    vkDestroyImageView = (PFN_vkDestroyImageView)vkGetDeviceProcAddr(device, "vkDestroyImageView");
    vkCreateCommandPool = (PFN_vkCreateCommandPool)vkGetDeviceProcAddr(
      device, "vkCreateCommandPool");
    vkResetCommandPool = (PFN_vkResetCommandPool)vkGetDeviceProcAddr(device, "vkResetCommandPool");
    vkDestroyCommandPool = (PFN_vkDestroyCommandPool)vkGetDeviceProcAddr(
      device, "vkDestroyCommandPool");
    vkAllocateCommandBuffers = (PFN_vkAllocateCommandBuffers)vkGetDeviceProcAddr(
      device, "vkAllocateCommandBuffers");
    vkCreateFence = (PFN_vkCreateFence)vkGetDeviceProcAddr(device, "vkCreateFence");
    vkWaitForFences = (PFN_vkWaitForFences)vkGetDeviceProcAddr(device, "vkWaitForFences");
    vkResetFences = (PFN_vkResetFences)vkGetDeviceProcAddr(device, "vkResetFences");
    vkDestroyFence = (PFN_vkDestroyFence)vkGetDeviceProcAddr(device, "vkDestroyFence");
    vkCreateSemaphore = (PFN_vkCreateSemaphore)vkGetDeviceProcAddr(device, "vkCreateSemaphore");
    vkDestroySemaphore = (PFN_vkDestroySemaphore)vkGetDeviceProcAddr(device, "vkDestroySemaphore");
    vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer)vkGetDeviceProcAddr(
      device, "vkBeginCommandBuffer");
    vkCmdPipelineBarrier2 = (PFN_vkCmdPipelineBarrier2)vkGetDeviceProcAddr(
      device, "vkCmdPipelineBarrier2");
    vkCmdClearColorImage = (PFN_vkCmdClearColorImage)vkGetDeviceProcAddr(
      device, "vkCmdClearColorImage");
    vkEndCommandBuffer = (PFN_vkEndCommandBuffer)vkGetDeviceProcAddr(device, "vkEndCommandBuffer");
    vkQueueSubmit2 = (PFN_vkQueueSubmit2)vkGetDeviceProcAddr(device, "vkQueueSubmit2");
    vkQueuePresentKHR = (PFN_vkQueuePresentKHR)vkGetDeviceProcAddr(device, "vkQueuePresentKHR");
}

void vulkan_release_global_functions(void) {
    vkCreateInstance = NULL;
    vkEnumerateInstanceLayerProperties = NULL;

#ifdef WIN32
    FreeLibrary(loaded_library);
#endif

    loaded_library = NULL;
}

void vulkan_release_instance_functions(void) {
    vkDestroyInstance = NULL;
    vkCreateDebugUtilsMessengerExt = NULL;
    vkDestroyDebugUtilsMessengerExt = NULL;
    vkEnumeratePhysicalDevices = NULL;
    vkGetPhysicalDeviceProperties = NULL;
    vkEnumerateDeviceExtensionProperties = NULL;
    vkGetPhysicalDeviceQueueFamilyProperties = NULL;
    vkGetPhysicalDeviceSurfaceSupportKHR = NULL;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR = NULL;
    vkGetPhysicalDeviceSurfaceFormatsKHR = NULL;
    vkGetPhysicalDeviceSurfacePresentModesKHR = NULL;
    vkDestroySurfaceKHR = NULL;
    vkCreateDevice = NULL;
    vkGetDeviceProcAddr = NULL;
}

void vulkan_release_device_functions(void) {
    vkDestroyDevice = NULL;
    vkDeviceWaitIdle = NULL;
    vkGetDeviceQueue = NULL;
    vkCreateSwapchainKHR = NULL;
    vkGetSwapchainImagesKHR = NULL;
    vkAcquireNextImageKHR = NULL;
    vkDestroySwapchainKHR = NULL;
    vkCreateImageView = NULL;
    vkDestroyImageView = NULL;
    vkCreateCommandPool = NULL;
    vkResetCommandPool = NULL;
    vkDestroyCommandPool = NULL;
    vkAllocateCommandBuffers = NULL;
    vkCreateFence = NULL;
    vkWaitForFences = NULL;
    vkResetFences = NULL;
    vkDestroyFence = NULL;
    vkCreateSemaphore = NULL;
    vkDestroySemaphore = NULL;
    vkBeginCommandBuffer = NULL;
    vkCmdPipelineBarrier2 = NULL;
    vkCmdClearColorImage = NULL;
    vkEndCommandBuffer = NULL;
    vkQueueSubmit2 = NULL;
    vkQueuePresentKHR = NULL;
}

// Global functions
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = NULL;
PFN_vkCreateInstance vkCreateInstance = NULL;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = NULL;

// Instance functions
PFN_vkDestroyInstance vkDestroyInstance = NULL;
PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerExt = NULL;
PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerExt = NULL;
PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = NULL;
PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties = NULL;
PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties = NULL;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = NULL;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR = NULL;
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR = NULL;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR = NULL;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR = NULL;
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = NULL;
PFN_vkCreateDevice vkCreateDevice = NULL;
PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = NULL;

// Device functions
PFN_vkDestroyDevice vkDestroyDevice = NULL;
PFN_vkDeviceWaitIdle vkDeviceWaitIdle = NULL;
PFN_vkGetDeviceQueue vkGetDeviceQueue = NULL;
PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR = NULL;
PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR = NULL;
PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR = NULL;
PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR = NULL;
PFN_vkCreateImageView vkCreateImageView = NULL;
PFN_vkDestroyImageView vkDestroyImageView = NULL;
PFN_vkCreateCommandPool vkCreateCommandPool = NULL;
PFN_vkResetCommandPool vkResetCommandPool = NULL;
PFN_vkDestroyCommandPool vkDestroyCommandPool = NULL;
PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers = NULL;
PFN_vkCreateFence vkCreateFence = NULL;
PFN_vkWaitForFences vkWaitForFences = NULL;
PFN_vkResetFences vkResetFences = NULL;
PFN_vkDestroyFence vkDestroyFence = NULL;
PFN_vkCreateSemaphore vkCreateSemaphore = NULL;
PFN_vkDestroySemaphore vkDestroySemaphore = NULL;
PFN_vkBeginCommandBuffer vkBeginCommandBuffer = NULL;
PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2 = NULL;
PFN_vkCmdClearColorImage vkCmdClearColorImage = NULL;
PFN_vkEndCommandBuffer vkEndCommandBuffer = NULL;
PFN_vkQueueSubmit2 vkQueueSubmit2 = NULL;
PFN_vkQueuePresentKHR vkQueuePresentKHR = NULL;
