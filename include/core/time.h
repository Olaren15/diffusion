#ifndef DIFFUSION_CORE_TIME_H
#define DIFFUSION_CORE_TIME_H

#include "stdint.h"

uint64_t get_elapsed_nanoseconds();

double nanoseconds_to_seconds(uint64_t nanoseconds);

#endif // DIFFUSION_CORE_TIME_H
