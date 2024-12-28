#include "core/dynamic_array.h"

#include <stdlib.h>
#include <string.h>

const size_t DEFAULT_ELEMENT_COUNT = 4;
const double GROWTH_FACTOR = 1.5;

void dynamic_array_push_multiple(dynamic_array_t* self, const void* elements, size_t element_count);

dynamic_array_t dynamic_array_allocate(size_t element_size) {
    size_t allocation_size = element_size * DEFAULT_ELEMENT_COUNT;
    void* data = malloc(allocation_size);

    if (data == NULL) {
        abort();
    }

    dynamic_array_t array = {
      .element_size = element_size,
      .element_count = 0,
      .allocated_bytes = allocation_size,
      .data = data,
    };

    return array;
}

dynamic_array_t dynamic_array_allocate_size(size_t element_size, size_t element_count) {
    size_t allocation_size = element_size * element_count;
    void* data = malloc(allocation_size);

    if (data == NULL) {
        abort();
    }

    dynamic_array_t array = {
      .element_size = element_size,
      .element_count = element_count,
      .allocated_bytes = allocation_size,
      .data = data,
    };

    return array;
}

void dynamic_array_free(dynamic_array_t* self) {
    free(self->data);
    self->data = NULL;
    self->element_size = 0;
    self->element_count = 0;
    self->allocated_bytes = 0;
}

void dynamic_array_push(dynamic_array_t* self, const void* element) {
    dynamic_array_push_multiple(self, element, 1);
}

void dynamic_array_concat(dynamic_array_t* self, const dynamic_array_t* other) {
    dynamic_array_push_multiple(self, other->data, other->element_count);
}

void dynamic_array_push_multiple(dynamic_array_t* self, const void* elements, size_t element_count) {
    const size_t new_required_bytes = element_count * self->element_size;
    const size_t currently_used_bytes = self->element_count * self->element_size;
    const size_t minimum_required_bytes = currently_used_bytes + new_required_bytes;

    size_t new_allocation_size = self->allocated_bytes;
    while (new_allocation_size < minimum_required_bytes) {
        new_allocation_size = (size_t)((double)new_allocation_size * GROWTH_FACTOR);
    }

    if (new_allocation_size != self->allocated_bytes) {
        void* new_data = realloc(self->data, new_allocation_size);

        if (new_data == NULL) {
            abort();
        }

        self->data = new_data;
        self->allocated_bytes = new_allocation_size;
    }

    void* new_items_location = (char*)self->data + (self->element_count * self->element_size);
    size_t bytes_to_copy = element_count * self->element_size;
    memcpy(new_items_location, elements, bytes_to_copy);

    self->element_count += element_count;
}
