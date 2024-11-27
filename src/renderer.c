#include "renderer.h"

#include "core/dynamic_array.h"
#include "project.h"

#include <vulkan/vulkan.h>

static const char* vk_surface_extension_name = VK_KHR_SURFACE_EXTENSION_NAME;
#ifdef WIN32
// clang-format off
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
// clang-format on
static const char* vk_platform_surface_extension_name = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
#else
#error Please define the platform-dependent vulkan surface extension name for your platform
#endif

static bool renderer_create_vk_instance(renderer_t* self);

bool renderer_init(renderer_t* self) {
    if (!renderer_create_vk_instance(self)) {
        return false;
    }

    return true;
}

void renderer_destroy(renderer_t* self) {
    vkDestroyInstance(self->vk_instance, NULL);
}

void renderer_render(const renderer_t* self) {
    // Do something
}

static bool renderer_create_vk_instance(renderer_t* self) {
    uint32_t engine_version = VK_MAKE_API_VERSION(
      PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH, 0
    );
    VkApplicationInfo application_info = {
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName = PROJECT_NAME,
      .applicationVersion = engine_version,
      .pEngineName = PROJECT_NAME,
      .engineVersion = engine_version,
      .apiVersion = VK_MAKE_API_VERSION(1, 0, 0, 0)
    };

    dynamic_array_t enabled_extensions = dynamic_array_allocate(sizeof(char*));
    dynamic_array_push(&enabled_extensions, &vk_surface_extension_name);
    dynamic_array_push(&enabled_extensions, &vk_platform_surface_extension_name);

    VkInstanceCreateInfo instance_create_info = {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &application_info,
      .enabledExtensionCount = (uint32_t)enabled_extensions.element_count,
      .ppEnabledExtensionNames = enabled_extensions.data,
    };

    VkResult result = vkCreateInstance(&instance_create_info, NULL, &self->vk_instance);
    if (result != VK_SUCCESS) {
        return false;
    }

    return true;
}
