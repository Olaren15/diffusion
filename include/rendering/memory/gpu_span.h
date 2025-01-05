#ifndef DIFFUSION_RENDERING_MEMORY_GPU_SPAN_H
#define DIFFUSION_RENDERING_MEMORY_GPU_SPAN_H

typedef struct gpu_span_s {
  VkDeviceSize offset;
  VkDeviceSize size;
} gpu_span_t;

#endif // DIFFUSION_RENDERING_MEMORY_GPU_SPAN_H
