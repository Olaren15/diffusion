#ifndef DIFFUSION_CORE_DYNAMIC_ARRAY_H
#define DIFFUSION_CORE_DYNAMIC_ARRAY_H

#include "core/slice.h"

#include <stdint.h>

typedef struct dynamic_array_s {
    size_t element_size;
    size_t element_count;
    size_t allocated_bytes;
    void* data;
} dynamic_array_t;

/**
 * Allocates a dynamic size array with default size
 *
 * @param element_size The size of one element in the array. Must not be zero.
 *
 * @returns The created dynamic array
 */
dynamic_array_t dynamic_array_allocate(size_t element_size);

/**
 * Allocates a dynamic size array with a specific size
 *
 * @param element_size The size of one element in the array. Must not be zero.
 * @param element_count The amount of elements to pre-allocate. Must not be zero.
 *
 * @returns The created dynamic array
 */
dynamic_array_t dynamic_array_allocate_size(size_t element_size, size_t element_count);

/**
 * Free the dynamic size array
 *
 * @param self The array to free
 */
void dynamic_array_free(dynamic_array_t* self);

dynamic_array_t dynamic_array_from(slice_t other);

/**
 * Push an element to the end of the array
 *
 * @param self The array to which the element should be added
 * @param element a pointer to the element that should be added
 */
void dynamic_array_push(dynamic_array_t* self, const void* element);

/**
 * Extends the array by `element_count`. The new elements are not initialised
 *
 * @returns the index where the new elements start
 */
size_t dynamic_array_extend(dynamic_array_t* self, size_t element_count);

void dynamic_array_concat(dynamic_array_t* self, const dynamic_array_t* other);

#endif // DIFFUSION_CORE_DYNAMIC_ARRAY_H
