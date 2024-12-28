#include "rendering/render_device.h"

#include <stdlib.h>

typedef struct device_infos_s {
    bool has_graphics_queue_family;
    uint32_t graphics_queue_family_index;
    bool has_present_queue_family;
    uint32_t present_queue_family_index;
    bool is_compatible_with_surface;
    bool supports_vulkan_1_3;
    VkPhysicalDevice vk_physical_device;
} device_infos_t;

static device_infos_t* pick_physical_device(const render_context_t* render_context);
static device_infos_t get_device_infos(VkPhysicalDevice device, VkSurfaceKHR surface);
static bool is_device_suitable(const device_infos_t* infos);

bool render_device_init(render_device_t* self, const render_context_t* render_context) {
    device_infos_t* chosen_device_infos = pick_physical_device(render_context);
    if (!chosen_device_infos) {
        return false;
    }

    dynamic_array_t queues_create_infos = dynamic_array_allocate(sizeof(VkDeviceQueueCreateInfo));
    float queue_priority = 1.0f;

    VkDeviceQueueCreateInfo graphics_queue_create_infos = {
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueFamilyIndex = chosen_device_infos->graphics_queue_family_index,
      .queueCount = 1,
      .pQueuePriorities = &queue_priority,
    };
    dynamic_array_push(&queues_create_infos, &graphics_queue_create_infos);

    if (chosen_device_infos->has_present_queue_family
        && chosen_device_infos->present_queue_family_index != chosen_device_infos->graphics_queue_family_index) {
        VkDeviceQueueCreateInfo present_queue_create_infos = {
          .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
          .queueFamilyIndex = chosen_device_infos->present_queue_family_index,
          .queueCount = 1,
          .pQueuePriorities = &queue_priority
        };
        dynamic_array_push(&queues_create_infos, &present_queue_create_infos);
    }

    VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures = {.alphaToOne = VK_FALSE};

    VkPhysicalDeviceVulkan13Features vulkan_1_3_features = {
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
      .dynamicRendering = VK_TRUE,
      .synchronization2 = VK_TRUE,
    };

    VkDeviceCreateInfo device_create_infos = {
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .queueCreateInfoCount = (uint32_t)queues_create_infos.element_count,
      .pQueueCreateInfos = queues_create_infos.data,
      .pEnabledFeatures = &vkPhysicalDeviceFeatures,
      .pNext = &vulkan_1_3_features,
    };

    if (vkCreateDevice(chosen_device_infos->vk_physical_device, &device_create_infos, NULL, &self->device)
        != VK_SUCCESS) {
        return false;
    }

    vulkan_load_device_functions(self->device);
    vkGetDeviceQueue(self->device, chosen_device_infos->graphics_queue_family_index, 0, &self->graphics_queue);

    self->present_queue = VK_NULL_HANDLE;
    if (chosen_device_infos->has_present_queue_family) {
        vkGetDeviceQueue(self->device, chosen_device_infos->present_queue_family_index, 0, &self->present_queue);
    }

    free(chosen_device_infos);
    dynamic_array_free(&queues_create_infos);

    return true;
}

void render_device_destroy(render_device_t* self) {
    vkDestroyDevice(self->device, NULL);
    vulkan_release_device_function();
    self->device = VK_NULL_HANDLE;
    self->graphics_queue = VK_NULL_HANDLE;
    self->present_queue = VK_NULL_HANDLE;
}

static device_infos_t* pick_physical_device(const render_context_t* render_context) {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(render_context->vk_instance, &device_count, NULL);

    if (device_count == 0) {
        return false;
    }

    device_infos_t* chosen_device_infos = malloc(sizeof(device_infos_t));
    dynamic_array_t physical_devices = dynamic_array_allocate_size(sizeof(VkPhysicalDevice), device_count);
    vkEnumeratePhysicalDevices(render_context->vk_instance, &device_count, physical_devices.data);

    for (size_t i = 0; i < physical_devices.element_count; i++) {
        VkPhysicalDevice device = ((VkPhysicalDevice*)physical_devices.data)[i];

        device_infos_t infos = get_device_infos(device, render_context->vk_surface);
        if (is_device_suitable(&infos)) {
            *chosen_device_infos = infos;
            break;
        }
    }

    dynamic_array_free(&physical_devices);

    return chosen_device_infos;
}

static device_infos_t get_device_infos(VkPhysicalDevice device, VkSurfaceKHR surface) {
    device_infos_t infos = {.vk_physical_device = device, .is_compatible_with_surface = surface == VK_NULL_HANDLE};

    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(device, &device_properties);
    if (VK_API_VERSION_MAJOR(device_properties.apiVersion) == 1
        && VK_API_VERSION_MINOR(device_properties.apiVersion) >= 3) {
        infos.supports_vulkan_1_3 = true;
    }

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);
    dynamic_array_t queue_families_props = dynamic_array_allocate_size(
      sizeof(VkQueueFamilyProperties), queue_family_count
    );
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families_props.data);

    for (size_t i = 0; i < queue_families_props.element_count; i++) {
        VkQueueFamilyProperties queue_family_properties = ((VkQueueFamilyProperties*)queue_families_props.data)[i];

        if (!infos.has_graphics_queue_family && queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            infos.has_graphics_queue_family = true;
            infos.graphics_queue_family_index = (uint32_t)i;
        }

        if (!infos.has_present_queue_family && surface != VK_NULL_HANDLE) {
            VkBool32 queue_family_supports_present;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, (uint32_t)i, surface, &queue_family_supports_present);

            if (queue_family_supports_present) {
                infos.has_present_queue_family = true;
                infos.present_queue_family_index = (uint32_t)i;
                infos.is_compatible_with_surface = true;
            }
        }
    }

    dynamic_array_free(&queue_families_props);

    return infos;
}

static bool is_device_suitable(const device_infos_t* infos) {
    return infos->has_graphics_queue_family && infos->is_compatible_with_surface && infos->supports_vulkan_1_3;
}
