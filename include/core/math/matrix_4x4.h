#ifndef DIFFUSION_INCLUDE_CORE_MATH_MATRIX_4X4_H
#define DIFFUSION_INCLUDE_CORE_MATH_MATRIX_4X4_H

#include "core/math/vector_3.h"

typedef struct matrix_4x4f_s {
    float entries[4][4];
} matrix_4x4f_t;

matrix_4x4f_t matrix_4x4f_multiply(matrix_4x4f_t lhs, matrix_4x4f_t rhs);

matrix_4x4f_t matrix_4x4f_translate(matrix_4x4f_t matrix, vector_3f_position_t translation_vector);

matrix_4x4f_t matrix_4x4f_projection(float vertical_fov, float aspect_ratio, float near, float far);

extern const matrix_4x4f_t matrix_4x4f_identity;

#endif // DIFFUSION_INCLUDE_CORE_MATH_MATRIX_4X4_H
