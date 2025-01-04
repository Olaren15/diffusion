#ifndef DIFFUSION_RENDERING_MODEL_VERTEX_H
#define DIFFUSION_RENDERING_MODEL_VERTEX_H

#include "core/math/vector_3.h"
#include "core/math/vector_4.h"

typedef struct vertex_s {
    vector_3f_position_t position;
    vector_4f_color_t color;
} vertex_t;

#endif // DIFFUSION_RENDERING_MODEL_VERTEX_H
