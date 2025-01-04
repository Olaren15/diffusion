#ifndef DIFFUSION_CORE_SLICE_H
#define DIFFUSION_CORE_SLICE_H

#include "stdint.h"

struct dynamic_array_s;

typedef struct slice_s {
    const void* first_element;
    size_t element_count;
    size_t element_size;
} slice_t;

slice_t slice_from_dynamic_array(const struct dynamic_array_s* array);

#endif // DIFFUSION_CORE_SLICE_H
