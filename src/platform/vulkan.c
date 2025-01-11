#include "platform/vulkan.h"

#ifdef _WIN32
#include "platform/win32/win32_types.h"

#include <vulkan/vulkan_win32.h>
#endif // _WIN32

static void* loaded_library = NULL;

bool vulkan_load_global_functions(void) {
#ifdef _WIN32
    HMODULE library = LoadLibraryW(L"vulkan-1.dll");
    if (library == NULL) {
        return false;
    }

    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)(void (*)(void))GetProcAddress(
      library, "vkGetInstanceProcAddr");
    loaded_library = library;
#endif // _WIN32

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
    vkDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)vkGetInstanceProcAddr(
      instance, "vkDestroySurfaceKHR");

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
    vkGetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties)
      vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties");

    vkCreateDevice = (PFN_vkCreateDevice)vkGetInstanceProcAddr(instance, "vkCreateDevice");
    vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)vkGetInstanceProcAddr(
      instance, "vkGetDeviceProcAddr");
}

void vulkan_load_device_functions(VkDevice device) {
    vkDestroyDevice = (PFN_vkDestroyDevice)vkGetDeviceProcAddr(device, "vkDestroyDevice");
    vkDeviceWaitIdle = (PFN_vkDeviceWaitIdle)vkGetDeviceProcAddr(device, "vkDeviceWaitIdle");

    vkAllocateMemory = (PFN_vkAllocateMemory)vkGetDeviceProcAddr(device, "vkAllocateMemory");
    vkFreeMemory = (PFN_vkFreeMemory)vkGetDeviceProcAddr(device, "vkFreeMemory");
    vkMapMemory = (PFN_vkMapMemory)vkGetDeviceProcAddr(device, "vkMapMemory");
    vkUnmapMemory = (PFN_vkUnmapMemory)vkGetDeviceProcAddr(device, "vkUnmapMemory");
    vkFlushMappedMemoryRanges = (PFN_vkFlushMappedMemoryRanges)vkGetDeviceProcAddr(
      device, "vkFlushMappedMemoryRanges");
    vkInvalidateMappedMemoryRanges = (PFN_vkInvalidateMappedMemoryRanges)vkGetDeviceProcAddr(
      device, "vkInvalidateMappedMemoryRanges");

    vkCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)vkGetDeviceProcAddr(
      device, "vkCreateSwapchainKHR");
    vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)vkGetDeviceProcAddr(
      device, "vkGetSwapchainImagesKHR");
    vkAcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)vkGetDeviceProcAddr(
      device, "vkAcquireNextImageKHR");
    vkDestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)vkGetDeviceProcAddr(
      device, "vkDestroySwapchainKHR");

    vkCreateBuffer = (PFN_vkCreateBuffer)vkGetDeviceProcAddr(device, "vkCreateBuffer");
    vkDestroyBuffer = (PFN_vkDestroyBuffer)vkGetDeviceProcAddr(device, "vkDestroyBuffer");
    vkGetBufferMemoryRequirements = (PFN_vkGetBufferMemoryRequirements)vkGetDeviceProcAddr(
      device, "vkGetBufferMemoryRequirements");
    vkBindBufferMemory = (PFN_vkBindBufferMemory)vkGetDeviceProcAddr(device, "vkBindBufferMemory");

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
    vkEndCommandBuffer = (PFN_vkEndCommandBuffer)vkGetDeviceProcAddr(device, "vkEndCommandBuffer");
    vkCmdPipelineBarrier2 = (PFN_vkCmdPipelineBarrier2)vkGetDeviceProcAddr(
      device, "vkCmdPipelineBarrier2");
    vkCmdClearColorImage = (PFN_vkCmdClearColorImage)vkGetDeviceProcAddr(
      device, "vkCmdClearColorImage");
    vkCmdBeginRendering = (PFN_vkCmdBeginRendering)vkGetDeviceProcAddr(
      device, "vkCmdBeginRendering");
    vkCmdEndRendering = (PFN_vkCmdEndRendering)vkGetDeviceProcAddr(device, "vkCmdEndRendering");
    vkCmdBindPipeline = (PFN_vkCmdBindPipeline)vkGetDeviceProcAddr(device, "vkCmdBindPipeline");
    vkCmdSetViewport = (PFN_vkCmdSetViewport)vkGetDeviceProcAddr(device, "vkCmdSetViewport");
    vkCmdSetScissor = (PFN_vkCmdSetScissor)vkGetDeviceProcAddr(device, "vkCmdSetScissor");
    vkCmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers)vkGetDeviceProcAddr(
      device, "vkCmdBindVertexBuffers");
    vkCmdBindIndexBuffer = (PFN_vkCmdBindIndexBuffer)vkGetDeviceProcAddr(
      device, "vkCmdBindIndexBuffer");
    vkCmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets)vkGetDeviceProcAddr(
      device, "vkCmdBindDescriptorSets");
    vkCmdDraw = (PFN_vkCmdDraw)vkGetDeviceProcAddr(device, "vkCmdDraw");
    vkCmdDrawIndexed = (PFN_vkCmdDrawIndexed)vkGetDeviceProcAddr(device, "vkCmdDrawIndexed");

    vkGetDeviceQueue = (PFN_vkGetDeviceQueue)vkGetDeviceProcAddr(device, "vkGetDeviceQueue");
    vkQueueSubmit2 = (PFN_vkQueueSubmit2)vkGetDeviceProcAddr(device, "vkQueueSubmit2");
    vkQueuePresentKHR = (PFN_vkQueuePresentKHR)vkGetDeviceProcAddr(device, "vkQueuePresentKHR");

    vkCreateDescriptorSetLayout = (PFN_vkCreateDescriptorSetLayout)vkGetDeviceProcAddr(
      device, "vkCreateDescriptorSetLayout");
    vkDestroyDescriptorSetLayout = (PFN_vkDestroyDescriptorSetLayout)vkGetDeviceProcAddr(
      device, "vkDestroyDescriptorSetLayout");
    vkCreateDescriptorPool = (PFN_vkCreateDescriptorPool)vkGetDeviceProcAddr(
      device, "vkCreateDescriptorPool");
    vkDestroyDescriptorPool = (PFN_vkDestroyDescriptorPool)vkGetDeviceProcAddr(
      device, "vkDestroyDescriptorPool");
    vkAllocateDescriptorSets = (PFN_vkAllocateDescriptorSets)vkGetDeviceProcAddr(
      device, "vkAllocateDescriptorSets");
    vkUpdateDescriptorSets = (PFN_vkUpdateDescriptorSets)vkGetDeviceProcAddr(
      device, "vkUpdateDescriptorSets");

    vkCreateShaderModule = (PFN_vkCreateShaderModule)vkGetDeviceProcAddr(
      device, "vkCreateShaderModule");
    vkDestroyShaderModule = (PFN_vkDestroyShaderModule)vkGetDeviceProcAddr(
      device, "vkDestroyShaderModule");
    vkCreatePipelineLayout = (PFN_vkCreatePipelineLayout)vkGetDeviceProcAddr(
      device, "vkCreatePipelineLayout");
    vkDestroyPipelineLayout = (PFN_vkDestroyPipelineLayout)vkGetDeviceProcAddr(
      device, "vkDestroyPipelineLayout");
    vkCreateGraphicsPipelines = (PFN_vkCreateGraphicsPipelines)vkGetDeviceProcAddr(
      device, "vkCreateGraphicsPipelines");
    vkDestroyPipeline = (PFN_vkDestroyPipeline)vkGetDeviceProcAddr(device, "vkDestroyPipeline");
}

void vulkan_release_global_functions(void) {
    vkCreateInstance = NULL;
    vkEnumerateInstanceLayerProperties = NULL;

#ifdef _WIN32
    FreeLibrary(loaded_library);
#endif // _WIN32

    loaded_library = NULL;
}

void vulkan_release_instance_functions(void) {
    vkDestroyInstance = NULL;
    vkDestroySurfaceKHR = NULL;

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
    vkGetPhysicalDeviceMemoryProperties = NULL;

    vkCreateDevice = NULL;
    vkGetDeviceProcAddr = NULL;
}

void vulkan_release_device_functions(void) {
    vkDestroyDevice = NULL;
    vkDeviceWaitIdle = NULL;

    vkAllocateMemory = NULL;
    vkFreeMemory = NULL;
    vkMapMemory = NULL;
    vkUnmapMemory = NULL;
    vkFlushMappedMemoryRanges = NULL;
    vkInvalidateMappedMemoryRanges = NULL;

    vkCreateSwapchainKHR = NULL;
    vkGetSwapchainImagesKHR = NULL;
    vkAcquireNextImageKHR = NULL;
    vkDestroySwapchainKHR = NULL;

    vkCreateBuffer = NULL;
    vkDestroyBuffer = NULL;
    vkGetBufferMemoryRequirements = NULL;
    vkBindBufferMemory = NULL;

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
    vkEndCommandBuffer = NULL;
    vkCmdPipelineBarrier2 = NULL;
    vkCmdClearColorImage = NULL;
    vkCmdBeginRendering = NULL;
    vkCmdEndRendering = NULL;
    vkCmdBindPipeline = NULL;
    vkCmdSetViewport = NULL;
    vkCmdSetScissor = NULL;
    vkCmdBindVertexBuffers = NULL;
    vkCmdBindIndexBuffer = NULL;
    vkCmdBindDescriptorSets = NULL;
    vkCmdDraw = NULL;
    vkCmdDrawIndexed = NULL;

    vkGetDeviceQueue = NULL;
    vkQueueSubmit2 = NULL;
    vkQueuePresentKHR = NULL;

    vkCreateDescriptorSetLayout = NULL;
    vkDestroyDescriptorSetLayout = NULL;
    vkCreateDescriptorPool = NULL;
    vkDestroyDescriptorPool = NULL;
    vkAllocateDescriptorSets = NULL;
    vkUpdateDescriptorSets = NULL;

    vkCreateShaderModule = NULL;
    vkDestroyShaderModule = NULL;
    vkCreatePipelineLayout = NULL;
    vkDestroyPipelineLayout = NULL;
    vkCreateGraphicsPipelines = NULL;
    vkDestroyPipeline = NULL;
}

// Global functions
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = NULL;
PFN_vkCreateInstance vkCreateInstance = NULL;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = NULL;

// Instance functions start
PFN_vkDestroyInstance vkDestroyInstance = NULL;
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = NULL;

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
PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties = NULL;

PFN_vkCreateDevice vkCreateDevice = NULL;
PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = NULL;
// Instance functions end

// Device functions
PFN_vkDestroyDevice vkDestroyDevice = NULL;
PFN_vkDeviceWaitIdle vkDeviceWaitIdle = NULL;

PFN_vkAllocateMemory vkAllocateMemory = NULL;
PFN_vkFreeMemory vkFreeMemory = NULL;
PFN_vkMapMemory vkMapMemory = NULL;
PFN_vkUnmapMemory vkUnmapMemory = NULL;
PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges = NULL;
PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges = NULL;

PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR = NULL;
PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR = NULL;
PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR = NULL;
PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR = NULL;

PFN_vkCreateBuffer vkCreateBuffer = NULL;
PFN_vkDestroyBuffer vkDestroyBuffer = NULL;
PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements = NULL;
PFN_vkBindBufferMemory vkBindBufferMemory = NULL;

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
PFN_vkEndCommandBuffer vkEndCommandBuffer = NULL;
PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2 = NULL;
PFN_vkCmdClearColorImage vkCmdClearColorImage = NULL;
PFN_vkCmdBeginRendering vkCmdBeginRendering = NULL;
PFN_vkCmdEndRendering vkCmdEndRendering = NULL;
PFN_vkCmdBindPipeline vkCmdBindPipeline = NULL;
PFN_vkCmdSetViewport vkCmdSetViewport = NULL;
PFN_vkCmdSetScissor vkCmdSetScissor = NULL;
PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers = NULL;
PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer = NULL;
PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets = NULL;
PFN_vkCmdDraw vkCmdDraw = NULL;
PFN_vkCmdDrawIndexed vkCmdDrawIndexed = NULL;

PFN_vkGetDeviceQueue vkGetDeviceQueue = NULL;
PFN_vkQueueSubmit2 vkQueueSubmit2 = NULL;
PFN_vkQueuePresentKHR vkQueuePresentKHR = NULL;

PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout = NULL;
PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout = NULL;
PFN_vkCreateDescriptorPool vkCreateDescriptorPool = NULL;
PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool = NULL;
PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets = NULL;
PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets = NULL;

PFN_vkCreateShaderModule vkCreateShaderModule = NULL;
PFN_vkDestroyShaderModule vkDestroyShaderModule = NULL;
PFN_vkCreatePipelineLayout vkCreatePipelineLayout = NULL;
PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout = NULL;
PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines = NULL;
PFN_vkDestroyPipeline vkDestroyPipeline = NULL;
// Device functions end
