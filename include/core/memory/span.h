#ifndef DIFFUSION_CORE_MEMORY_SPAN_H
#define DIFFUSION_CORE_MEMORY_SPAN_H

#include <stdint.h>

struct dynamic_array_s;

typedef struct span_s {
    const void* start;
    const size_t size;
} span_t;

span_t span_from_dynamic_array(const struct dynamic_array_s* array);

#endif // DIFFUSION_CORE_MEMORY_SPAN_H
