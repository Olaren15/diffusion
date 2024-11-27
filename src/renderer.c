#include "renderer.h"

#include "core/dynamic_array.h"
#include "platform/vulkan.h"
#include "project.h"

static bool renderer_create_vk_instance(renderer_t* self);

bool renderer_init(renderer_t* self) {
    if (!vulkan_load_global_functions()) {
        return false;
    }

    if (!renderer_create_vk_instance(self)) {
        return false;
    }

    vulkan_load_instance_functions(self->vk_instance);

    return true;
}

void renderer_destroy(renderer_t* self) {
    vkDestroyInstance(self->vk_instance, NULL);
    vulkan_release_functions();
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

    dynamic_array_t enabled_extensions = vulkan_get_required_extensions_for_presentation();
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

    dynamic_array_free(&enabled_extensions);

    return true;
}
