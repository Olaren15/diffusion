#include "core/memory/dynamic_array_element_reference.h"

void* follow_dynamic_array_reference(dynamic_array_element_reference_t reference) {
    return (char*)reference.array->data + reference.index;
}
