#include "core/memory/span.h"

#include "core/memory/dynamic_array.h"

span_t span_from_dynamic_array(const dynamic_array_t* array) {
    span_t span = {
      .start = array->data,
      .size = array->element_count * array->element_size,
    };

    return span;
}
