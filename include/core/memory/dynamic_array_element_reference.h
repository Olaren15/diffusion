#ifndef DIFFUSION_CORE_MEMORY_DYNAMIC_ARRAY_ELEMENT_REFERENCE_H
#define DIFFUSION_CORE_MEMORY_DYNAMIC_ARRAY_ELEMENT_REFERENCE_H

#include "core/memory/dynamic_array.h"

/**
 * A reference to an item stored in a dynamic_array_t.
 * This struct holds a pointer a dynamic array and must not outlive it
 */
typedef struct dynamic_array_element_reference_s {
    dynamic_array_t* array;
    size_t index;
} dynamic_array_element_reference_t;

/**
 * Returns a pointer to the referenced element
 */
void* follow_dynamic_array_reference(dynamic_array_element_reference_t reference);

#endif // DIFFUSION_CORE_MEMORY_DYNAMIC_ARRAY_ELEMENT_REFERENCE_H
