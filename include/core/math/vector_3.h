#ifndef DIFFUSION_CORE_MATH_VECTOR_3_H
#define DIFFUSION_CORE_MATH_VECTOR_3_H

/**
 * A 3-dimensional floating-point position vector that complies with std430 alignment
 */
typedef struct vector_3f_position_s {
    float x;
    float y;
    float z;
    float _padding;
} vector_3f_position_t;

/**
 * A 3-dimensional floating-point color vector that complies with std430 alignment
 */
typedef struct vector_3f_color_s {
    float r;
    float g;
    float b;
    float _padding;
} vector_3f_color_t;

/**
 * A generic 3-dimensional floating-point vector that complies with std430 alignment
 */
typedef union vector_3f_u {
    vector_3f_position_t position;
    vector_3f_color_t color;
} vector_3f_t;

vector_3f_t vector_3f_substract(vector_3f_t lhs, vector_3f_t rhs);
vector_3f_t vector_3f_divide(vector_3f_t vector, float divider);
vector_3f_t vector_3f_multiply(vector_3f_t vector, float product);

float vector_3f_dot_product(vector_3f_t lhs, vector_3f_t rhs);
vector_3f_t vector_3f_cross_product(vector_3f_t lhs, vector_3f_t rhs);

float vector_3f_magnitude(vector_3f_t vector);
vector_3f_t vector_3f_normalise(vector_3f_t vector);

#endif // DIFFUSION_CORE_MATH_VECTOR_3_H
