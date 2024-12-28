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

    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)(void (*)(void))GetProcAddress(library, "vkGetInstanceProcAddr");
    loaded_library = library;
#endif

    if (vkGetInstanceProcAddr == NULL) {
        return false;
    }

    vkCreateInstance = (PFN_vkCreateInstance)vkGetInstanceProcAddr(NULL, "vkCreateInstance");
    vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties
    )vkGetInstanceProcAddr(NULL, "vkEnumerateInstanceLayerProperties");

    return true;
}

void vulkan_load_instance_functions(VkInstance instance) {
    vkDestroyInstance = (PFN_vkDestroyInstance)vkGetInstanceProcAddr(instance, "vkDestroyInstance");
    vkCreateDebugUtilsMessengerExt = (PFN_vkCreateDebugUtilsMessengerEXT
    )vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    vkDestroyDebugUtilsMessengerExt = (PFN_vkDestroyDebugUtilsMessengerEXT
    )vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices
    )vkGetInstanceProcAddr(instance, "vkEnumeratePhysicalDevices");
    vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties
    )vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties");
    vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties
    )vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties");
    vkGetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR
    )vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
    vkDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)vkGetInstanceProcAddr(instance, "vkDestroySurfaceKHR");
    vkCreateDevice = (PFN_vkCreateDevice)vkGetInstanceProcAddr(instance, "vkCreateDevice");
    vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)vkGetInstanceProcAddr(instance, "vkGetDeviceProcAddr");
}

void vulkan_load_device_functions(VkDevice device) {
    vkDestroyDevice = (PFN_vkDestroyDevice)vkGetDeviceProcAddr(device, "vkDestroyDevice");
    vkGetDeviceQueue = (PFN_vkGetDeviceQueue)vkGetDeviceProcAddr(device, "vkGetDeviceQueue");
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
    vkGetPhysicalDeviceQueueFamilyProperties = NULL;
    vkGetPhysicalDeviceSurfaceSupportKHR = NULL;
    vkDestroySurfaceKHR = NULL;
    vkCreateDevice = NULL;
    vkGetDeviceProcAddr = NULL;
}

void vulkan_release_device_function(void) {
    vkDestroyDevice = NULL;
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
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = NULL;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR = NULL;
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = NULL;
PFN_vkCreateDevice vkCreateDevice = NULL;
PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = NULL;

// Device functions
PFN_vkDestroyDevice vkDestroyDevice = NULL;
PFN_vkGetDeviceQueue vkGetDeviceQueue = NULL;
