﻿#include "platform/vulkan.h"

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
    vkCreateDebugUtilsMessengerExt = (PFN_vkCreateDebugUtilsMessengerEXT)(void (*)(void)
    )vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    vkDestroyDebugUtilsMessengerExt = (PFN_vkDestroyDebugUtilsMessengerEXT)(void (*)(void)
    )vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
}

void vulkan_load_device_functions(VkDevice device) {
}

void vulkan_release_functions(void) {
    vkCreateInstance = NULL;
    vkEnumerateInstanceLayerProperties = NULL;

    vkDestroyInstance = NULL;
    vkCreateDebugUtilsMessengerExt = NULL;

#ifdef WIN32
    FreeLibrary(loaded_library);
#endif

    loaded_library = NULL;
}

dynamic_array_t vulkan_get_required_extensions_for_presentation(void) {
    dynamic_array_t required_extensions = dynamic_array_allocate(sizeof(const char*));

    static const char* vk_surface_extension_name = VK_KHR_SURFACE_EXTENSION_NAME;
    dynamic_array_push(&required_extensions, &vk_surface_extension_name);

#ifdef WIN32
    static const char* vk_platform_surface_extension_name = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
    dynamic_array_push(&required_extensions, &vk_platform_surface_extension_name);
#else
#endif

    return required_extensions;
}

// Global functions
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = NULL;
PFN_vkCreateInstance vkCreateInstance = NULL;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = NULL;

// Instance functions
PFN_vkDestroyInstance vkDestroyInstance = NULL;
PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerExt = NULL;
PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerExt = NULL;
