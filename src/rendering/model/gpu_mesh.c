#include "rendering/model/gpu_mesh.h"

bool gpu_mesh_create(
  gpu_mesh_t* self,
  const mesh_t* mesh,
  const render_device_t* device,
  rebar_gpu_allocator_t* allocator) {

    VkBufferCreateInfo vertex_buffer_create_infos = {
      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .size = sizeof(vertex_t) * mesh->vertices.element_count,
    };

    if (!rebar_gpu_allocator_create_buffer(
          allocator, device, &vertex_buffer_create_infos, &self->vertex_buffer)) {
        return false;
    }

    if (!gpu_allocated_buffer_copy_data(
          &self->vertex_buffer, device, slice_from_dynamic_array(&mesh->vertices))) {
        return false;
    }
    self->vertex_count = mesh->vertices.element_count;

    VkBufferCreateInfo index_buffer_create_info = {
      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .size = sizeof(uint32_t) * mesh->indices.element_count,
    };

    if (!rebar_gpu_allocator_create_buffer(
          allocator, device, &index_buffer_create_info, &self->index_buffer)) {
        return false;
    }

    if (!gpu_allocated_buffer_copy_data(
          &self->index_buffer, device, slice_from_dynamic_array(&mesh->indices))) {
        return false;
    }
    self->index_count = mesh->indices.element_count;

    return true;
}

void gpu_mesh_destroy(
  gpu_mesh_t* self, const render_device_t* device, rebar_gpu_allocator_t* allocator) {
    vkDestroyBuffer(device->vk_device, self->vertex_buffer.buffer, NULL);
    vkDestroyBuffer(device->vk_device, self->index_buffer.buffer, NULL);
}
