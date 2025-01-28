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
    double fov_radians = vertical_fov * M_PI / 180.0;

    double focal_length = 1 / tan(fov_radians / 2.0);
    float x = (float)(focal_length / aspect_ratio);
    float y = (float)(-focal_length);
    float A = (float)(near / (far - near));
    float B = (float)(far * A);

    // clang-format off
    matrix_4x4f_t value = {
      .entries = {
        x   , 0.0f, 0.0f, 0.0f,
        0.0f, y   , 0.0f, 0.0f,
        0.0f, 0.0f, A   , B   ,
        0.0f, 0.0f, 1.0f, 0.0f,
      }
    };
    // clang-format on

    return value;
}

matrix_4x4f_t matrix_4x4f_look_at(vector_3f_t camera_position, vector_3f_t target, vector_3f_t up) {
    vector_3f_t forward = vector_3f_normalise(vector_3f_substract(camera_position, target));
    vector_3f_t right = vector_3f_normalise(vector_3f_cross_product(up, forward));
    vector_3f_t relative_up = vector_3f_normalise(vector_3f_cross_product(forward, right));

    float pos_x = -vector_3f_dot_product(right, camera_position);
    float pos_y = -vector_3f_dot_product(up, camera_position);
    float pos_z = vector_3f_dot_product(forward, camera_position);

    // clang-format off
    matrix_4x4f_t value = {
      .entries = {
        right.position.x, relative_up.position.x, forward.position.x, pos_x,
        right.position.y, relative_up.position.y, forward.position.y, pos_y,
        right.position.z, relative_up.position.z, forward.position.z, pos_z,
        0.0f            , 0.0f                  , 0.0f              , 1.0f ,
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
