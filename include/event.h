#ifndef DIFFUSION_EVENT_H
#define DIFFUSION_EVENT_H

typedef enum event_type_e {
    EVENT_TYPE_UNKNOWN,
    EVENT_TYPE_QUIT,
} event_type_t;

typedef struct event_s {
    event_type_t type;
} event_t;

#endif // DIFFUSION_EVENT_H
