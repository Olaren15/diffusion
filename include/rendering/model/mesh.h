#ifndef DIFFUSION_RENDERING_MODEL_MESH_H
#define DIFFUSION_RENDERING_MODEL_MESH_H

#include "rendering/model/vertex.h"
#include "core/memory/dynamic_array.h"

typedef struct mesh_s {
    dynamic_array_t vertices; // vertex_t
    dynamic_array_t indices; // uint32_t
} mesh_t;

#endif // DIFFUSION_RENDERING_MODEL_MESH_H
