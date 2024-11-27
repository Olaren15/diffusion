#include "platform/vulkan.h"

#ifdef WIN32
// clang-format off
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
// clang-format on
#endif


static void* loaded_library = NULL;

bool vulkan_load_global_functions(void) {
#ifdef WIN32
    HMODULE library = LoadLibraryW(L"vulkan-1.dll");
    if (library == NULL) {
        return false;
    }

    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(library, "vkGetInstanceProcAddr");
    loaded_library = library;
#endif

    if (vkGetInstanceProcAddr == NULL) {
        return false;
    }

    vkCreateInstance = (PFN_vkCreateInstance)vkGetInstanceProcAddr(NULL, "vkCreateInstance");
    return true;
}

void vulkan_load_instance_functions(VkInstance instance) {
    vkDestroyInstance = (PFN_vkDestroyInstance)vkGetInstanceProcAddr(instance, "vkDestroyInstance");
}

void vulkan_load_device_functions(VkDevice device) {
}

void vulkan_release_functions(void) {
    vkCreateInstance = NULL;
    vkDestroyInstance = NULL;

#ifdef WIN32
    FreeLibrary(loaded_library);
#endif

    loaded_library = NULL;
}

dynamic_array_t vulkan_get_required_extensions_for_presentation() {
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

// Instance functions
PFN_vkDestroyInstance vkDestroyInstance = NULL;
