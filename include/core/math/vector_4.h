#ifndef DIFFUSION_CORE_MATH_VECTOR_4_H
#define DIFFUSION_CORE_MATH_VECTOR_4_H

/**
 * A 4-dimensional floating-point position vector that complies with std430 alignment
 */
typedef struct vector_4f_position_s {
    float x;
    float y;
    float z;
    float w;
} vector_4f_position_t;

/**
 * A 4-dimensional floating-point color vector that complies with std430 alignment
 */
typedef struct vector_4f_color_s {
    float r;
    float g;
    float b;
    float a;
} vector_4f_color_t;

/**
 * A generic 4-dimensional floating-point vector that complies with std430 alignment
 */
typedef union vector_4f_u {
    vector_4f_position_t position;
    vector_4f_color_t color;
} vector_4f_t;

#endif //DIFFUSION_CORE_MATH_VECTOR_4_H
