#include "rendering/memory/rebar_gpu_allocator.h"

const VkDeviceSize DEFAULT_ALLOCATION_SIZE = 256 * 1024 * 1024; // 256 Mib

static bool find_rebar_memory_type(const render_device_t* device, uint32_t* memory_type_index);

bool rebar_gpu_allocator_available(const render_device_t* device) {
    uint32_t memory_type_index;
    return find_rebar_memory_type(device, &memory_type_index);
}

bool rebar_gpu_allocator_init(rebar_gpu_allocator_t* self, const render_device_t* device) {
    if (!find_rebar_memory_type(device, &self->memory_type_index)) {
        return false;
    }

    self->rebar_memory_type = device->memory_properties.memoryTypes[self->memory_type_index];
    self->allocations = dynamic_array_allocate(sizeof(gpu_allocation_t));

    return true;
}

void rebar_gpu_allocator_destroy(rebar_gpu_allocator_t* self, const render_device_t* device) {
    for (size_t i = 0; i < self->allocations.element_count; i++) {
        gpu_allocation_t* allocation = ((gpu_allocation_t*)self->allocations.data) + i;
        gpu_allocation_destroy(allocation, device);
    }

    dynamic_array_free(&self->allocations);
}

/**
 * Finds the resizable bar memory type and assigns it to the memory_type_index param.
 *
 * @returns true if the memory type was found, false if not found
 */
bool find_rebar_memory_type(const render_device_t* device, uint32_t* memory_type_index) {
    for (uint32_t i = 0; i < device->memory_properties.memoryTypeCount; i++) {
        VkMemoryType current_memory_type = device->memory_properties.memoryTypes[i];

        if (
          (current_memory_type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
            == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
          && (current_memory_type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
               == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
          && (current_memory_type.propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
               == VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
            *memory_type_index = i;
            return true;
        }
    }

    return false;
}

bool rebar_gpu_allocator_create_buffer(
  rebar_gpu_allocator_t* self,
  const render_device_t* device,
  const VkBufferCreateInfo* buffer_create_info,
  gpu_allocated_buffer_t* buffer) {

    VkBuffer vk_buffer;
    if (vkCreateBuffer(device->vk_device, buffer_create_info, NULL, &vk_buffer) != VK_SUCCESS) {
        return false;
    }

    VkMemoryRequirements memory_requirements;
    vkGetBufferMemoryRequirements(device->vk_device, vk_buffer, &memory_requirements);

    // Check if buffer can be held in our memory type
    if ((self->rebar_memory_type.propertyFlags & memory_requirements.memoryTypeBits) == 0) {
        vkDestroyBuffer(device->vk_device, vk_buffer, NULL);
        return false;
    }

    gpu_span_t memory_span;
    gpu_allocation_t* allocation = NULL;
    for (size_t i = 0; i < self->allocations.element_count; i++) {
        gpu_allocation_t* potential_allocation = ((gpu_allocation_t*)self->allocations.data) + i;

        if (gpu_allocation_can_sub_allocate(
              potential_allocation, memory_requirements.size, memory_requirements.alignment)) {

            allocation = potential_allocation;
            memory_span = gpu_allocation_sub_allocate(
              potential_allocation, memory_requirements.size, memory_requirements.alignment);
        }
    }

    if (allocation == NULL) {
        VkDeviceSize allocation_size = memory_requirements.size > DEFAULT_ALLOCATION_SIZE
                                         ? memory_requirements.size
                                         : DEFAULT_ALLOCATION_SIZE;

        size_t new_allocation_index = dynamic_array_extend(&self->allocations, 1);
        allocation = ((gpu_allocation_t*)self->allocations.data) + new_allocation_index;
        if (!gpu_allocation_create(allocation, device, self->memory_type_index, allocation_size)) {

            vkDestroyBuffer(device->vk_device, vk_buffer, NULL);
            return false;
        }

        memory_span = gpu_allocation_sub_allocate(
          allocation, memory_requirements.size, memory_requirements.alignment);
    }

    if (
      vkBindBufferMemory(device->vk_device, vk_buffer, allocation->memory, memory_span.offset)
      != VK_SUCCESS) {
        vkDestroyBuffer(device->vk_device, vk_buffer, NULL);
        return false;
    }

    *buffer = (gpu_allocated_buffer_t){
      .vk_buffer = vk_buffer,
      .span = memory_span,
      .allocation = allocation,
    };

    return true;
}
