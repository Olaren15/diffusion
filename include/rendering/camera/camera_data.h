#ifndef DIFFUSION_RENDERING_CAMERA_CAMERA_DATA_H
#define DIFFUSION_RENDERING_CAMERA_CAMERA_DATA_H

#include "core/math/matrix_4x4.h"

typedef struct camera_data_s {
    matrix_4x4f_t view;
    matrix_4x4f_t projection;
    matrix_4x4f_t view_projection;
} camera_data_t;

#endif // DIFFUSION_RENDERING_CAMERA_CAMERA_DATA_H
