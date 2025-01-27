#include "core/math/matrix_4x4.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define NUMBER_OF_ROWS 4
#define NUMBER_OF_COLUMNS 4

matrix_4x4f_t matrix_4x4f_multiply(matrix_4x4f_t lhs, matrix_4x4f_t rhs) {
    matrix_4x4f_t result = {0};

    for (size_t i = 0; i < NUMBER_OF_ROWS; i++) {
        for (size_t j = 0; j < NUMBER_OF_COLUMNS; j++) {
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

matrix_4x4f_t matrix_4x4f_projection(
  float vertical_fov, float aspect_ratio, float near, float far) {
    double fov_radians = vertical_fov * 2.0 * M_PI / 360.0;
    double a = tan(fov_radians / 2.0);
    double b = far / (far - near);

    float x = (float)(a / aspect_ratio);
    float y = (float)a;
    float z = (float)b;
    float zt = (float)(-near * b);

    // clang-format off
    matrix_4x4f_t value = {
      .entries = {
        x   , 0.0f, 0.0f, 0.0f,
        0.0f, y   , 0.0f, 0.0f,
        0.0f, 0.0f, z   , zt  ,
        0.0f, 0.0f, 1.0f, 0.0f,
      }
    };
    // clang-format on

    return value;
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
