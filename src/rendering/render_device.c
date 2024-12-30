#include "rendering/render_device.h"

#include <string.h>

typedef struct device_infos_s {
    bool has_graphics_queue_family;
    uint32_t graphics_queue_family_index;
    bool has_present_queue_family;
    uint32_t present_queue_family_index;
    present_capabilities_t present_capabilities;
    bool supports_required_extensions;
    bool supports_minimum_vulkan_version;
    VkPhysicalDevice vk_physical_device;
} device_infos_t;

static const char* swapchain_extension_name = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

static bool pick_physical_device(
  const render_context_t* render_context,
  const dynamic_array_t* required_extensions,
  device_infos_t* chosen_device_infos
);
static device_infos_t
  get_device_infos(VkPhysicalDevice device, VkSurfaceKHR surface, const dynamic_array_t* required_extensions);
static void fill_device_vulkan_version_support(VkPhysicalDevice device, device_infos_t* device_infos);
static void fill_device_extension_support(
  VkPhysicalDevice device, const dynamic_array_t* required_extensions, device_infos_t* device_infos
);
static void fill_device_queues_support(VkPhysicalDevice device, VkSurfaceKHR surface, device_infos_t* device_infos);
static void
  fill_device_present_capabilities(VkPhysicalDevice device, VkSurfaceKHR surface, device_infos_t* device_infos);
static bool is_device_suitable(const device_infos_t* infos, bool verify_present_capabilities);

bool render_device_init(render_device_t* self, const render_context_t* render_context) {
    dynamic_array_t required_extensions = dynamic_array_allocate(sizeof(const char*));
    if (render_context->vk_surface != VK_NULL_HANDLE) {
        dynamic_array_push(&required_extensions, &swapchain_extension_name);
    }

    device_infos_t chosen_device_infos;
    if (!pick_physical_device(render_context, &required_extensions, &chosen_device_infos)) {
        return false;
    }

    dynamic_array_t queues_create_infos = dynamic_array_allocate(sizeof(VkDeviceQueueCreateInfo));
    float queue_priority = 1.0f;

    VkDeviceQueueCreateInfo graphics_queue_create_infos = {
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueFamilyIndex = chosen_device_infos.graphics_queue_family_index,
      .queueCount = 1,
      .pQueuePriorities = &queue_priority,
    };
    dynamic_array_push(&queues_create_infos, &graphics_queue_create_infos);

    if (chosen_device_infos.has_present_queue_family
        && chosen_device_infos.present_queue_family_index != chosen_device_infos.graphics_queue_family_index) {
        VkDeviceQueueCreateInfo present_queue_create_infos = {
          .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
          .queueFamilyIndex = chosen_device_infos.present_queue_family_index,
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
      .enabledExtensionCount = (uint32_t)required_extensions.element_count,
      .ppEnabledExtensionNames = required_extensions.data,
      .pEnabledFeatures = &vkPhysicalDeviceFeatures,
      .pNext = &vulkan_1_3_features,
    };

    if (vkCreateDevice(chosen_device_infos.vk_physical_device, &device_create_infos, NULL, &self->vk_device)
        != VK_SUCCESS) {
        return false;
    }

    vulkan_load_device_functions(self->vk_device);

    self->present_capabilities = chosen_device_infos.present_capabilities;

    self->graphics_queue_family_index = chosen_device_infos.graphics_queue_family_index;
    vkGetDeviceQueue(self->vk_device, chosen_device_infos.graphics_queue_family_index, 0, &self->graphics_queue);

    self->present_queue = VK_NULL_HANDLE;
    if (chosen_device_infos.has_present_queue_family) {
        self->present_queue_family_index = chosen_device_infos.present_queue_family_index;
        vkGetDeviceQueue(self->vk_device, chosen_device_infos.present_queue_family_index, 0, &self->present_queue);
    }

    dynamic_array_free(&queues_create_infos);
    dynamic_array_free(&required_extensions);

    return true;
}

void render_device_destroy(render_device_t* self) {
    vkDeviceWaitIdle(self->vk_device);

    vkDestroyDevice(self->vk_device, NULL);
    vulkan_release_device_functions();

    dynamic_array_free(&self->present_capabilities.supported_present_modes);
    dynamic_array_free(&self->present_capabilities.supported_surface_formats);

    self->vk_device = VK_NULL_HANDLE;
    self->graphics_queue = VK_NULL_HANDLE;
    self->present_queue = VK_NULL_HANDLE;
}

static bool pick_physical_device(
  const render_context_t* render_context,
  const dynamic_array_t* required_extensions,
  device_infos_t* chosen_device_infos
) {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(render_context->vk_instance, &device_count, NULL);

    if (device_count == 0) {
        return false;
    }

    dynamic_array_t physical_devices = dynamic_array_allocate_size(sizeof(VkPhysicalDevice), device_count);
    vkEnumeratePhysicalDevices(render_context->vk_instance, &device_count, physical_devices.data);

    bool check_presentation_support = render_context->vk_surface != VK_NULL_HANDLE;
    for (size_t i = 0; i < physical_devices.element_count; i++) {
        VkPhysicalDevice device = ((VkPhysicalDevice*)physical_devices.data)[i];

        device_infos_t infos = get_device_infos(device, render_context->vk_surface, required_extensions);
        if (is_device_suitable(&infos, check_presentation_support)) {
            *chosen_device_infos = infos;
            break;
        }
    }

    dynamic_array_free(&physical_devices);

    return true;
}

static device_infos_t
  get_device_infos(VkPhysicalDevice device, VkSurfaceKHR surface, const dynamic_array_t* required_extensions) {
    device_infos_t infos = {
      .vk_physical_device = device,
    };

    fill_device_vulkan_version_support(device, &infos);
    fill_device_extension_support(device, required_extensions, &infos);
    fill_device_queues_support(device, surface, &infos);
    if (surface != VK_NULL_HANDLE) {
        fill_device_present_capabilities(device, surface, &infos);
    }

    return infos;
}

static void fill_device_vulkan_version_support(VkPhysicalDevice device, device_infos_t* device_infos) {
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(device, &device_properties);
    if (VK_API_VERSION_MAJOR(device_properties.apiVersion) == 1
        && VK_API_VERSION_MINOR(device_properties.apiVersion) >= 3) {
        device_infos->supports_minimum_vulkan_version = true;
    }
}

static void fill_device_extension_support(
  VkPhysicalDevice device, const dynamic_array_t* required_extensions, device_infos_t* device_infos
) {
    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, NULL);
    dynamic_array_t device_extensions = dynamic_array_allocate_size(sizeof(VkExtensionProperties), extension_count);
    vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, device_extensions.data);

    bool missing_extension = false;
    for (size_t i = 0; i < required_extensions->element_count && !missing_extension; i++) {
        const char* required_extension = ((const char**)required_extensions->data)[i];
        bool found = false;

        for (size_t j = 0; j < device_extensions.element_count; j++) {
            VkExtensionProperties extension = ((VkExtensionProperties*)device_extensions.data)[j];
            if (strcmp(required_extension, extension.extensionName) == 0) {
                found = true;
                break;
            }
        }

        missing_extension |= !found;
    }

    device_infos->supports_required_extensions = !missing_extension;

    dynamic_array_free(&device_extensions);
}

static void fill_device_queues_support(VkPhysicalDevice device, VkSurfaceKHR surface, device_infos_t* device_infos) {
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);
    dynamic_array_t queue_families_props = dynamic_array_allocate_size(
      sizeof(VkQueueFamilyProperties), queue_family_count
    );
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families_props.data);

    for (size_t i = 0; i < queue_families_props.element_count; i++) {
        VkQueueFamilyProperties queue_family_properties = ((VkQueueFamilyProperties*)queue_families_props.data)[i];

        if (!device_infos->has_graphics_queue_family && queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            device_infos->has_graphics_queue_family = true;
            device_infos->graphics_queue_family_index = (uint32_t)i;
        }

        if (!device_infos->has_present_queue_family && surface != VK_NULL_HANDLE) {
            VkBool32 queue_family_supports_present_to_surface;
            vkGetPhysicalDeviceSurfaceSupportKHR(
              device, (uint32_t)i, surface, &queue_family_supports_present_to_surface
            );

            if (queue_family_supports_present_to_surface) {
                device_infos->has_present_queue_family = true;
                device_infos->present_queue_family_index = (uint32_t)i;
            }
        }
    }

    dynamic_array_free(&queue_families_props);
}

static void
  fill_device_present_capabilities(VkPhysicalDevice device, VkSurfaceKHR surface, device_infos_t* device_infos) {
    present_capabilities_t present_capabilities;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &present_capabilities.vk_surface_capabilities);

    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, NULL);
    present_capabilities.supported_surface_formats = dynamic_array_allocate_size(
      sizeof(VkSurfaceFormatKHR), format_count
    );
    vkGetPhysicalDeviceSurfaceFormatsKHR(
      device, surface, &format_count, present_capabilities.supported_surface_formats.data
    );

    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, NULL);
    present_capabilities.supported_present_modes = dynamic_array_allocate_size(
      sizeof(VkPresentModeKHR), present_mode_count
    );
    vkGetPhysicalDeviceSurfacePresentModesKHR(
      device, surface, &present_mode_count, present_capabilities.supported_present_modes.data
    );

    device_infos->present_capabilities = present_capabilities;
}

static bool is_device_suitable(const device_infos_t* infos, bool verify_present_capabilities) {
    bool is_suitable = true;

    is_suitable &= infos->has_graphics_queue_family;
    is_suitable &= infos->supports_minimum_vulkan_version;
    is_suitable &= infos->supports_required_extensions;

    if (verify_present_capabilities) {
        is_suitable &= infos->has_present_queue_family;
        is_suitable &= infos->present_capabilities.supported_surface_formats.element_count >= 1;
        is_suitable &= infos->present_capabilities.supported_present_modes.element_count >= 1;
    }

    return is_suitable;
}
