﻿#ifndef DIFFUSION_RENDERING_CAMERA_GPU_CAMERA_H
#define DIFFUSION_RENDERING_CAMERA_GPU_CAMERA_H

#include "rendering/memory/gpu_allocated_buffer.h"
#include "rendering/memory/rebar_gpu_allocator.h"
#include "rendering/render_device.h"
#include "rendering/scene/scene_data.h"

typedef struct frame_gpu_data_s {
    scene_data_t scene_data;
    VkDescriptorSet descriptor_set;
    gpu_allocated_buffer_t uniform_buffer;
} frame_gpu_data_t;

bool frame_gpu_data_init(
  frame_gpu_data_t* self,
  const render_device_t* device,
  rebar_gpu_allocator_t* allocator,
  VkDescriptorSetLayout scene_descriptor_set_layout,
  VkDescriptorPool descriptor_pool);

void frame_gpu_data_destroy(
  frame_gpu_data_t* self, const render_device_t* device, rebar_gpu_allocator_t* allocator);

bool frame_gpu_data_upload_to_gpu(frame_gpu_data_t* self, const render_device_t* device);

#endif // DIFFUSION_RENDERING_CAMERA_GPU_CAMERA_H
