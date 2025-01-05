#include "rendering/model/gpu_mesh.h"

#include "core/memory/span.h"

bool gpu_mesh_create(
  gpu_mesh_t* self,
  const mesh_t* mesh,
  const render_device_t* device,
  rebar_gpu_allocator_t* allocator) {

    span_t indices_source_span = span_from_dynamic_array(&mesh->indices);
    span_t vertices_source_span = span_from_dynamic_array(&mesh->vertices);

    self->index_count = mesh->indices.element_count;
    self->indices_span = (gpu_span_t){
      .offset = 0,
      .size = indices_source_span.size,
    };
    self->vertices_span = (gpu_span_t){
      .offset = indices_source_span.size,
      .size = vertices_source_span.size,
    };

    VkBufferCreateInfo buffer_create_infos = {
      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .size = indices_source_span.size + vertices_source_span.size,
    };

    if (!rebar_gpu_allocator_create_buffer(
          allocator, device, &buffer_create_infos, &self->buffer)) {
        return false;
    }

    if (!gpu_allocated_buffer_copy_data(
          &self->buffer, device, self->indices_span.offset, indices_source_span)) {
        return false;
    }

    if (!gpu_allocated_buffer_copy_data(
          &self->buffer, device, self->vertices_span.offset, vertices_source_span)) {
        return false;
    }

    return true;
}

void gpu_mesh_destroy(
  gpu_mesh_t* self, const render_device_t* device, rebar_gpu_allocator_t* allocator) {
    vkDestroyBuffer(device->vk_device, self->buffer.vk_buffer, NULL);
}

void gpu_mesh_draw(const gpu_mesh_t* self, VkCommandBuffer command_buffer) {
    vkCmdBindVertexBuffers(
      command_buffer, 0, 1, &self->buffer.vk_buffer, &self->vertices_span.offset);
    vkCmdBindIndexBuffer(
      command_buffer, self->buffer.vk_buffer, self->indices_span.offset, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(command_buffer, self->index_count, 1, 0, 0, 0);
}
