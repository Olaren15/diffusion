#ifndef DIFFUSION_ENGINE_H
#define DIFFUSION_ENGINE_H

#include "platform/window.h"
#include "rendering/render_context.h"
#include "rendering/render_device.h"
#include "rendering/swapchain.h"

#include <stdbool.h>

typedef struct engine_s {
    window_t window;
    render_context_t render_context;
    render_device_t render_device;
    swapchain_t swapchain;
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
void engine_run(const engine_t* self);

#endif // DIFFUSION_ENGINE_H
