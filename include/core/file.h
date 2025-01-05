#ifndef DIFFUSION_CORE_FILE_H
#define DIFFUSION_CORE_FILE_H

#include "core/memory/dynamic_array.h"

/**
 * Read a whole file to a byte buffer
 *
 * @returns A buffer with the file data. NULL on error
 */
dynamic_array_t read_file(const char* file_name);

#endif // DIFFUSION_CORE_FILE_H
