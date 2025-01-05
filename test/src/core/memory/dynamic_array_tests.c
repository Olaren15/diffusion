#include "core/memory/dynamic_array.h"
#include "test.h"

TEST_DATA_IMPL;

TEST_CASE(size__dynamic_array_allocate__allocates_for_at_least_one_element) {
    // given
    const size_t element_size = sizeof(float);

    // when
    dynamic_array_t result = dynamic_array_allocate(element_size);

    // then
    ASSERT_EQUALS(element_size, result.element_size);
    ASSERT_EQUALS(0, result.element_count);
    ASSERT_TRUE(element_size <= result.allocated_bytes);
    ASSERT_NOT_NULL(result.data);

    dynamic_array_free(&result);
    TEST_SUCCESS;
}

TEST_CASE(size_and_count__dynamic_array_allocate_size__allocates_array_and_initialises_fields) {
    // given
    const size_t element_size = sizeof(uint32_t);
    const size_t element_count = 10;
    const size_t expected_allocated_bytes = element_size * element_count;

    // when
    dynamic_array_t result = dynamic_array_allocate_size(element_size, element_count);

    // then
    ASSERT_EQUALS(element_size, result.element_size);
    ASSERT_EQUALS(element_count, result.element_count);
    ASSERT_EQUALS(expected_allocated_bytes, result.allocated_bytes);
    ASSERT_NOT_NULL(result.data);

    dynamic_array_free(&result);
    TEST_SUCCESS;
}

TEST_CASE(dynamic_array__dynamic_array_free__deallocates_memory_and_resets_fields) {
    // given
    dynamic_array_t dynamic_array = dynamic_array_allocate_size(sizeof(float), 14);

    // when
    dynamic_array_free(&dynamic_array);

    // then
    ASSERT_EQUALS(0, dynamic_array.element_size);
    ASSERT_EQUALS(0, dynamic_array.element_count);
    ASSERT_EQUALS(0, dynamic_array.allocated_bytes);
    ASSERT_NULL(dynamic_array.data);

    TEST_SUCCESS;
}

TEST_CASE(array_too_small__dynamic_array_push__reallocates_memory_to_fit_new_item) {
    // given
    const size_t element_size = sizeof(char*);
    const size_t initial_element_count = 3;
    const char* element_to_add = "test";
    const size_t expected_element_count = 4;
    dynamic_array_t dynamic_array = dynamic_array_allocate_size(
      element_size, initial_element_count);

    // when
    dynamic_array_push(&dynamic_array, &element_to_add);

    // then
    ASSERT_EQUALS(element_size, dynamic_array.element_size);
    ASSERT_EQUALS(expected_element_count, dynamic_array.element_count);
    ASSERT_STRING_EQUALS(element_to_add, ((char**)dynamic_array.data)[3]);
    ASSERT_NOT_NULL(dynamic_array.data);

    dynamic_array_free(&dynamic_array);
    TEST_SUCCESS;
}

TEST_CASE(array_with_free_space_dynamic_array_push__does_not_reallocate) {
    // given
    const size_t element_size = sizeof(char*);
    const size_t initial_element_count = 3;
    const char* element_to_add = "test";
    dynamic_array_t dynamic_array = dynamic_array_allocate_size(
      element_size, initial_element_count);
    dynamic_array.element_count--; // Simulate an array with one free space

    // when
    dynamic_array_push(&dynamic_array, &element_to_add);

    // then
    ASSERT_EQUALS(element_size, dynamic_array.element_size);
    ASSERT_EQUALS(initial_element_count, dynamic_array.element_count);
    ASSERT_STRING_EQUALS(element_to_add, ((char**)dynamic_array.data)[2]);
    ASSERT_NOT_NULL(dynamic_array.data);

    dynamic_array_free(&dynamic_array);
    TEST_SUCCESS;
}

TEST_CASE(array_too_small__dynamic_array_concat__reallocates_memory_to_fit_new_items) {
    // given
    const size_t element_size = sizeof(char*);
    const size_t initial_element_count = 3;
    dynamic_array_t other = dynamic_array_allocate_size(element_size, 2);
    const char* element_to_add_1 = "test";
    ((const char**)other.data)[0] = element_to_add_1;
    const char* element_to_add_2 = "test2";
    ((const char**)other.data)[1] = element_to_add_2;
    const size_t expected_element_count = 5;
    dynamic_array_t dynamic_array = dynamic_array_allocate_size(
      element_size, initial_element_count);

    // when
    dynamic_array_concat(&dynamic_array, &other);

    // then
    ASSERT_EQUALS(element_size, dynamic_array.element_size);
    ASSERT_EQUALS(expected_element_count, dynamic_array.element_count);
    ASSERT_STRING_EQUALS(element_to_add_1, ((char**)dynamic_array.data)[3]);
    ASSERT_STRING_EQUALS(element_to_add_2, ((char**)dynamic_array.data)[4]);
    ASSERT_NOT_NULL(dynamic_array.data);

    dynamic_array_free(&dynamic_array);
    dynamic_array_free(&other);
    TEST_SUCCESS;
}

TEST_CASE(array_with_free_space_dynamic_array_concat__does_not_reallocate) {
    // given
    const size_t element_size = sizeof(char*);
    const size_t initial_element_count = 4;
    dynamic_array_t other = dynamic_array_allocate_size(element_size, 2);
    const char* element_to_add_1 = "test";
    ((const char**)other.data)[0] = element_to_add_1;
    const char* element_to_add_2 = "test2";
    ((const char**)other.data)[1] = element_to_add_2;
    dynamic_array_t dynamic_array = dynamic_array_allocate_size(
      element_size, initial_element_count);
    dynamic_array.element_count -= 2; // Simulate an array with two free spaces

    // when
    dynamic_array_concat(&dynamic_array, &other);

    // then
    ASSERT_EQUALS(element_size, dynamic_array.element_size);
    ASSERT_EQUALS(initial_element_count, dynamic_array.element_count);
    ASSERT_STRING_EQUALS(element_to_add_1, ((char**)dynamic_array.data)[2]);
    ASSERT_STRING_EQUALS(element_to_add_2, ((char**)dynamic_array.data)[3]);
    ASSERT_NOT_NULL(dynamic_array.data);

    dynamic_array_free(&dynamic_array);
    dynamic_array_free(&other);
    TEST_SUCCESS;
}

TEST_RUN_ALL {
    TEST_RUN(size__dynamic_array_allocate__allocates_for_at_least_one_element);
    TEST_RUN(size_and_count__dynamic_array_allocate_size__allocates_array_and_initialises_fields);
    TEST_RUN(dynamic_array__dynamic_array_free__deallocates_memory_and_resets_fields);
    TEST_RUN(array_too_small__dynamic_array_push__reallocates_memory_to_fit_new_item);
    TEST_RUN(array_with_free_space_dynamic_array_push__does_not_reallocate);
    TEST_RUN(array_too_small__dynamic_array_concat__reallocates_memory_to_fit_new_items);
    TEST_RUN(array_with_free_space_dynamic_array_concat__does_not_reallocate);
}

TEST_MAIN_IMPL
