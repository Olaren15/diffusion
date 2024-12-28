#include "engine.h"

#include "project.h"

const int DEFAULT_WIDTH = 1920;
const int DEFAULT_HEIGHT = 1080;

/*
 * Do one iteration (frame) worth of stuff
 *
 * @returns Whether the app should continue running
 */
static bool engine_iterate(const engine_t* self);

bool engine_init(engine_t* self) {
    if (!window_create(PROJECT_NAME, DEFAULT_WIDTH, DEFAULT_HEIGHT, &self->window)) {
        return false;
    }

    if (!render_context_init(&self->render_context, &self->window)) {
        return false;
    }

    if (!render_device_init(&self->render_device, &self->render_context)) {
        return false;
    }

    return true;
}

void engine_destroy(engine_t* self) {
    render_device_destroy(&self->render_device);
    render_context_destroy(&self->render_context);
    window_destroy(&self->window);
}

void engine_run(const engine_t* self) {
    bool continue_running = true;
    event_t event;

    while (continue_running) {
        while (continue_running && window_poll_event(&self->window, &event)) {
            continue_running &= event.type != EVENT_TYPE_QUIT;
        }

        if (continue_running) {
            continue_running &= engine_iterate(self);
        }
    }
}

static bool engine_iterate(const engine_t* self) {
    (void)self; // Unused for now
    return true;
}
