#ifndef DIFFUSION_ENGINE_H
#define DIFFUSION_ENGINE_H

#include "platform/window.h"
#include "rendering/frame/frame.h"
#include "rendering/memory/gpu_allocated_buffer.h"
#include "rendering/memory/rebar_gpu_allocator.h"
#include "rendering/model/gpu_mesh.h"
#include "rendering/render_context.h"
#include "rendering/render_device.h"
#include "rendering/swapchain.h"
#include "rendering/triangle_pipeline.h"

#include <stdbool.h>

#define ENGINE_MAX_FRAMES_IN_FLIGHT 2

typedef struct engine_s {
    window_t window;
    render_context_t render_context;
    render_device_t render_device;
    rebar_gpu_allocator_t gpu_allocator;
    swapchain_t swapchain;
    uint32_t current_frame;
    frame_t frames[ENGINE_MAX_FRAMES_IN_FLIGHT];
    VkDescriptorSetLayout scene_descriptor_set_layout;
    VkDescriptorPool descriptor_pool;
    triangle_pipeline_t triangle_pipeline;
    gpu_mesh_t triangle_mesh;
} engine_t;

/**
 * Initialize the engine
 *
 * @returns true on success or false on failure
 */
bool engine_init(engine_t* self);

/**
 * Destroy the engine and clean up
 */
void engine_destroy(engine_t* self);

/**
 * Run the engine
 */
void engine_run(engine_t* self);

#endif // DIFFUSION_ENGINE_H
