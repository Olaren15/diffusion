#include "core/math/matrix_4x4.h"
#include "test.h"

TEST_DATA_IMPL;

TEST_CASE(matrix_4x4f_and_identity__matrix_4x4f_multiply__returns_original_matrix) {
    // given
    // clang-format off
    matrix_4x4f_t matrix = {
        .entries = {
             5.5f,  144.22f, 2.3f,   -45.32f,
            -15.0f, 5.6f,    34.82f, -89.1f,
             4.5f,  7.8f,    1.21f,   0.6f,
             72.1f, 17.48f,  0.33f,   1.0f,
        }
    };
    // clang-format on

    // when
    matrix_4x4f_t actual = matrix_4x4f_multiply(matrix, matrix_4x4f_identity);

    // then
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            ASSERT_EQUALS(matrix.entries[i][j], actual.entries[i][j]);
        }
    }

    TEST_SUCCESS;
}

TEST_CASE(identity_and_translation_vector__matrix_4x4f_translate__returns_matrix_with_translation) {
    // given
    vector_3f_position_t translation = {.x = 5.0f, .y = 4.4f, .z = 2.3f};

    // when
    matrix_4x4f_t actual = matrix_4x4f_translate(matrix_4x4f_identity, translation);

    // then
    ASSERT_EQUALS(actual.entries[0][3], translation.x);
    ASSERT_EQUALS(actual.entries[1][3], translation.y);
    ASSERT_EQUALS(actual.entries[2][3], translation.z);
    ASSERT_EQUALS(actual.entries[3][3], 1.0f);

    TEST_SUCCESS;
}

TEST_RUN_ALL {
    TEST_RUN(matrix_4x4f_and_identity__matrix_4x4f_multiply__returns_original_matrix);
    TEST_RUN(
      identity_and_translation_vector__matrix_4x4f_translate__returns_matrix_with_translation);
}

TEST_MAIN_IMPL
