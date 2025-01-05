#include "core/memory/slice.h"

#include "core/memory/dynamic_array.h"

slice_t slice_from_dynamic_array(const dynamic_array_t* array) {
    slice_t slice = {
      .element_size = array->element_size,
      .element_count = array->element_count,
      .first_element = array->data,
    };

    return slice;
}
