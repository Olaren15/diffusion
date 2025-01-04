#ifndef DIFFUSION_CORE_MATH_VECTOR_H
#define DIFFUSION_CORE_MATH_VECTOR_H

/**
 * A 2-dimensional floating-point position vector that complies with std430 alignment
 */
typedef struct vector_2f_position_s {
    float x;
    float y;
} vector_2f_position_t;

/**
 * A 4-dimensional floating-point color vector that complies with std430 alignment
 */
typedef struct vector_2f_color_s {
    float r;
    float g;
} vector_2f_color_t;

/**
 * A generic 4-dimensional floating-point vector that complies with std430 alignment
 */
typedef union vector_2f_u {
    vector_2f_position_t position;
    vector_2f_color_t color;
} vector_2f_t;

#endif // DIFFUSION_CORE_MATH_VECTOR_H
