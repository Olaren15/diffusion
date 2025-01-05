#ifndef DIFFUSION_RENDERING_MODEL_GPU_MESH_H
#define DIFFUSION_RENDERING_MODEL_GPU_MESH_H

#include "platform/vulkan.h"
#include "rendering/memory/gpu_allocated_buffer.h"
#include "rendering/memory/gpu_span.h"
#include "rendering/memory/rebar_gpu_allocator.h"
#include "rendering/model/mesh.h"
#include "rendering/render_device.h"

typedef struct gpu_mesh_s {
    gpu_allocated_buffer_t buffer;
    gpu_span_t indices_span;
    gpu_span_t vertices_span;
    size_t index_count;
} gpu_mesh_t;

bool gpu_mesh_create(
  gpu_mesh_t* self,
  const mesh_t* mesh,
  const render_device_t* device,
  rebar_gpu_allocator_t* allocator);

void gpu_mesh_destroy(
  gpu_mesh_t* self, const render_device_t* device, rebar_gpu_allocator_t* allocator);

void gpu_mesh_draw(const gpu_mesh_t* self, VkCommandBuffer command_buffer);

#endif // DIFFUSION_RENDERING_MODEL_GPU_MESH_H
