#include "core/math/matrix_4x4.h"

matrix_4x4f_t matrix_4x4f_multiply(matrix_4x4f_t lhs, matrix_4x4f_t rhs) {
    matrix_4x4f_t result = {0};

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            result.entries[i][j] = lhs.entries[i][0] * rhs.entries[0][j]
                                   + lhs.entries[i][1] * rhs.entries[1][j]
                                   + lhs.entries[i][2] * rhs.entries[2][j]
                                   + lhs.entries[i][3] * rhs.entries[3][j];
        }
    }

    return result;
}

matrix_4x4f_t matrix_4x4f_translate(matrix_4x4f_t matrix, vector_3f_position_t translation_vector) {
    matrix_4x4f_t translation_matrix = matrix_4x4f_identity;
    translation_matrix.entries[0][3] = translation_vector.x;
    translation_matrix.entries[1][3] = translation_vector.y;
    translation_matrix.entries[2][3] = translation_vector.z;

    return matrix_4x4f_multiply(matrix, translation_matrix);
}

// clang-format off
const matrix_4x4f_t matrix_4x4f_identity = {
  .entries = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f,
  }
};
// clang-format on
