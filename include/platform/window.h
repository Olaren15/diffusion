#ifndef DIFFUSION_PLATFORM_WINDOW_H
#define DIFFUSION_PLATFORM_WINDOW_H

#include "event.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct platform_window_s platform_window_t;

typedef struct window_s {
    platform_window_t* platform_window;
} window_t;

/**
 * Initialize a platform-specific window
 *
 * @param title The title of the window to create
 * @param width The width (in pixels) for the created window
 * @param height The height (in pixels) for the created window
 * @param window A pointer to a new window struct that will contain the new window's data
 *
 * @returns true on success or false on failure
 */
bool window_create(const char* title, int width, int height, window_t* window);

/**
 * Destroy a platform-specific window
 *
 * @param self The window to be destroyed
 */
void window_destroy(window_t* self);

/**
 * Poll the window for new events
 *
 * @param self The window to poll
 * @param event A pointer to the event variable to be filled with the new event
 *
 * @returns if there are other events left to process
 */
bool window_poll_event(const window_t* self, event_t* event);

#endif // DIFFUSION_PLATFORM_WINDOW_H
