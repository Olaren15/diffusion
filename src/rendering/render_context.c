#include "rendering/render_context.h"

#include "core/dynamic_array.h"
#include "platform/vulkan.h"
#include "project.h"

#include <stdio.h>
#include <string.h>

#ifdef NDEBUG
const bool validation_layers_requested = false;
#else
const bool validation_layers_requested = true;
#endif

const char* validation_layer_name = "VK_LAYER_KHRONOS_validation";
const char* vk_ext_debug_utils_extension_name = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

static bool should_enable_validation_layers(void);
static bool is_layer_available(const dynamic_array_t* available_layers, const char* layer_name);
static VkDebugUtilsMessengerCreateInfoEXT build_debug_utils_messenger_create_info(void);
static bool render_context_create_vk_instance(render_context_t* self, const window_t* window);
static bool render_context_create_debug_messenger_callback(render_context_t* self);
static VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debug_callback(
  VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
  VkDebugUtilsMessageTypeFlagsEXT message_type,
  const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
  void* user_data
);

bool render_context_init(render_context_t* self, const window_t* window) {
    if (!vulkan_load_global_functions()) {
        return false;
    }

    self->validation_layers_enabled = should_enable_validation_layers();

    if (!render_context_create_vk_instance(self, window)) {
        return false;
    }

    vulkan_load_instance_functions(self->vk_instance);

    if (!render_context_create_debug_messenger_callback(self)) {
        return false;
    }

    if (window != NULL) {
        if (!window_create_vk_surface(window, self->vk_instance, &self->vk_surface)) {
            return false;
        }
    } else {
        self->vk_surface = VK_NULL_HANDLE;
    }

    return true;
}

void render_context_destroy(render_context_t* self) {
    vkDestroySurfaceKHR(self->vk_instance, self->vk_surface, NULL);

    if (vkDestroyDebugUtilsMessengerExt != NULL && self->debug_messenger != NULL) {
        vkDestroyDebugUtilsMessengerExt(self->vk_instance, self->debug_messenger, NULL);
        self->debug_messenger = VK_NULL_HANDLE;
    }

    vkDestroyInstance(self->vk_instance, NULL);
    self->vk_instance = VK_NULL_HANDLE;

    vulkan_release_instance_functions();
    vulkan_release_global_functions();
}

static bool should_enable_validation_layers(void) {
    if (!validation_layers_requested) {
        return false;
    }

    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, NULL);

    dynamic_array_t available_instance_layers = dynamic_array_allocate_size(sizeof(VkLayerProperties), layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_instance_layers.data);

    bool validation_layers_available = is_layer_available(&available_instance_layers, validation_layer_name);

    dynamic_array_free(&available_instance_layers);
    return validation_layers_available;
}

static bool is_layer_available(const dynamic_array_t* available_layers, const char* layer_name) {
    for (size_t i = 0; i < available_layers->element_count; i++) {
        VkLayerProperties layer_properties = ((VkLayerProperties*)available_layers->data)[i];
        if (strcmp(layer_name, layer_properties.layerName) == 0) {
            return true;
        }
    }

    return false;
}

static VkDebugUtilsMessengerCreateInfoEXT build_debug_utils_messenger_create_info(void) {
    VkDebugUtilsMessengerCreateInfoEXT create_infos = {
      .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
      .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                         | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                         | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
      .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                     | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
                     | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,
      .pfnUserCallback = vulkan_debug_callback
    };

    return create_infos;
}

static bool render_context_create_vk_instance(render_context_t* self, const window_t* window) {
    uint32_t engine_version = VK_MAKE_API_VERSION(
      PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH, 0
    );
    VkApplicationInfo application_info = {
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName = PROJECT_NAME,
      .applicationVersion = engine_version,
      .pEngineName = PROJECT_NAME,
      .engineVersion = engine_version,
      .apiVersion = VK_API_VERSION_1_3,
    };

    dynamic_array_t enabled_extensions = dynamic_array_allocate(sizeof(const char*));
    if (window != NULL) {
        dynamic_array_t presentation_extensions = window_get_required_extensions_for_presentation();
        dynamic_array_concat(&enabled_extensions, &presentation_extensions);
        dynamic_array_free(&presentation_extensions);
    }
    dynamic_array_t enabled_layers = dynamic_array_allocate(sizeof(const char*));

    void* next = NULL;
    VkDebugUtilsMessengerCreateInfoEXT debug_utils_messegner_create_info = build_debug_utils_messenger_create_info();

    if (self->validation_layers_enabled) {
        dynamic_array_push(&enabled_layers, &validation_layer_name);
        dynamic_array_push(&enabled_extensions, &vk_ext_debug_utils_extension_name);
        next = &debug_utils_messegner_create_info;
    }

    VkInstanceCreateInfo instance_create_info = {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &application_info,
      .enabledExtensionCount = (uint32_t)enabled_extensions.element_count,
      .ppEnabledExtensionNames = enabled_extensions.data,
      .enabledLayerCount = (uint32_t)enabled_layers.element_count,
      .ppEnabledLayerNames = enabled_layers.data,
      .pNext = next,
    };

    VkResult result = vkCreateInstance(&instance_create_info, NULL, &self->vk_instance);
    if (result != VK_SUCCESS) {
        return false;
    }

    dynamic_array_free(&enabled_layers);
    dynamic_array_free(&enabled_extensions);

    return true;
}

static bool render_context_create_debug_messenger_callback(render_context_t* self) {
    if (!self->validation_layers_enabled) {
        self->debug_messenger = NULL;
        return true;
    }

    VkDebugUtilsMessengerCreateInfoEXT create_info = build_debug_utils_messenger_create_info();

    VkResult result = vkCreateDebugUtilsMessengerExt(self->vk_instance, &create_info, NULL, &self->debug_messenger);
    return result == VK_SUCCESS;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debug_callback(
  VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
  VkDebugUtilsMessageTypeFlagsEXT message_type,
  const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
  void* user_data
) {
    // Unused variables
    (void)message_type;
    (void)user_data;

    const char* severity_level_text;
    switch (message_severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            severity_level_text = "VERBOSE";
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            severity_level_text = "INFO";
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            severity_level_text = "WARNING";
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            severity_level_text = "ERROR";
            break;
        default:
            severity_level_text = "UNKNOWN";
            break;
    }

    printf("[%s] [Vulkan]: %s\n", severity_level_text, callback_data->pMessage);

    return VK_FALSE;
}
