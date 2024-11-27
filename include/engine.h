#ifndef DIFFUSION_ENGINE_H
#define DIFFUSION_ENGINE_H

#include "platform/window.h"
#include "renderer.h"

#include <stdbool.h>

typedef struct engine_s {
    window_t window;
    renderer_t renderer;
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
