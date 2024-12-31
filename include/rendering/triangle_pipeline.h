#ifndef DIFFUSION_RENDERING_TRIANGLE_PIPELINE_H
#define DIFFUSION_RENDERING_TRIANGLE_PIPELINE_H

#include "platform/vulkan.h"
#include "rendering/render_device.h"

typedef struct triangle_pipeline_s {
    VkPipelineLayout layout;
    VkPipeline vk_pipeline;
} triangle_pipeline_t;

/**
 * Create a pipeline that renders a triangle
 */
bool triangle_pipeline_create(triangle_pipeline_t* self, const render_device_t* device, VkFormat color_attachment_format);

/**
 * Destroy the triangle pipeline
 */
void triangle_pipeline_destroy(triangle_pipeline_t* self, const render_device_t* device);

#endif // DIFFUSION_RENDERING_TRIANGLE_PIPELINE_H
