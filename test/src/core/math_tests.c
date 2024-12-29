#include "core/math.h"
#include "test.h"

TEST_DATA_IMPL;

TEST_CASE(a_lesser_than_b__min_uint32_t__returns_a) {
    // given
    uint32_t a = 3;
    uint32_t b = 4;

    // when
    uint32_t actual = min_uint32_t(a, b);

    // then
    ASSERT_EQUALS(a, actual);

    TEST_SUCCESS;
}

TEST_CASE(a_higher_than_b__min_uint32_t__returns_b) {
    // given
    uint32_t a = 6;
    uint32_t b = 4;

    // when
    uint32_t actual = min_uint32_t(a, b);

    // then
    ASSERT_EQUALS(b, actual);

    TEST_SUCCESS;
}

TEST_CASE(a_equals_b__min_uint32_t__returns_a_and_b) {
    // given
    uint32_t a = 4;
    uint32_t b = 4;

    // when
    uint32_t actual = min_uint32_t(a, b);

    // then
    ASSERT_EQUALS(a, actual);
    ASSERT_EQUALS(b, actual);

    TEST_SUCCESS;
}

TEST_CASE(a_lesser_than_b__max_uint32_t__returns_b) {
    // given
    uint32_t a = 3;
    uint32_t b = 4;

    // when
    uint32_t actual = max_uint32_t(a, b);

    // then
    ASSERT_EQUALS(b, actual);

    TEST_SUCCESS;
}

TEST_CASE(a_higher_than_b__max_uint32_t__returns_a) {
    // given
    uint32_t a = 6;
    uint32_t b = 4;

    // when
    uint32_t actual = max_uint32_t(a, b);

    // then
    ASSERT_EQUALS(a, actual);

    TEST_SUCCESS;
}

TEST_CASE(a_equals_b__max_uint32_t__returns_a_and_b) {
    // given
    uint32_t a = 4;
    uint32_t b = 4;

    // when
    uint32_t actual = min_uint32_t(a, b);

    // then
    ASSERT_EQUALS(a, actual);
    ASSERT_EQUALS(b, actual);

    TEST_SUCCESS;
}

TEST_CASE(value_between_min_and_max__clamp_uint32_t__returns_value) {
    // given
    uint32_t min = 3;
    uint32_t max = 5;
    uint32_t value = 4;

    // when
    uint32_t actual = clamp_uint32_t(min, max, value);

    // then
    ASSERT_EQUALS(value, actual);

    TEST_SUCCESS;
}

TEST_CASE(value_equals_min__clamp_uint32_t__returns_min_and_value) {
    // given
    uint32_t min = 3;
    uint32_t max = 5;
    uint32_t value = 3;

    // when
    uint32_t actual = clamp_uint32_t(min, max, value);

    // then
    ASSERT_EQUALS(min, actual);
    ASSERT_EQUALS(value, actual);

    TEST_SUCCESS;
}

TEST_CASE(value_lesser_than_min__clamp_uint32_t__returns_min) {
    // given
    uint32_t min = 3;
    uint32_t max = 5;
    uint32_t value = 2;

    // when
    uint32_t actual = clamp_uint32_t(min, max, value);

    // then
    ASSERT_EQUALS(min, actual);

    TEST_SUCCESS;
}

TEST_CASE(value_equals_max__clamp_uint32_t__returns_max_and_value) {
    // given
    uint32_t min = 3;
    uint32_t max = 5;
    uint32_t value = 5;

    // when
    uint32_t actual = clamp_uint32_t(min, max, value);

    // then
    ASSERT_EQUALS(max, actual);
    ASSERT_EQUALS(value, actual);

    TEST_SUCCESS;
}

TEST_CASE(value_higher_than_max__clamp_uint32_t__returns_max) {
    // given
    uint32_t min = 3;
    uint32_t max = 5;
    uint32_t value = 6;

    // when
    uint32_t actual = clamp_uint32_t(min, max, value);

    // then
    ASSERT_EQUALS(max, actual);

    TEST_SUCCESS;
}

TEST_RUN_ALL {
    TEST_RUN(a_lesser_than_b__min_uint32_t__returns_a);
    TEST_RUN(a_higher_than_b__min_uint32_t__returns_b);
    TEST_RUN(a_equals_b__min_uint32_t__returns_a_and_b);
    TEST_RUN(a_lesser_than_b__max_uint32_t__returns_b);
    TEST_RUN(a_higher_than_b__max_uint32_t__returns_a);
    TEST_RUN(a_equals_b__max_uint32_t__returns_a_and_b);
    TEST_RUN(value_between_min_and_max__clamp_uint32_t__returns_value);
    TEST_RUN(value_equals_min__clamp_uint32_t__returns_min_and_value);
    TEST_RUN(value_lesser_than_min__clamp_uint32_t__returns_min);
    TEST_RUN(value_equals_max__clamp_uint32_t__returns_max_and_value);
    TEST_RUN(value_higher_than_max__clamp_uint32_t__returns_max);
}

TEST_MAIN_IMPL
