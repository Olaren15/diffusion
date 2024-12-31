#include "core/file.h"

#include <stdio.h>

dynamic_array_t read_file(const char* file_name) {
    dynamic_array_t buffer = {
      .data = NULL,
      .element_count = 0,
      .element_size = sizeof(char),
      .allocated_bytes = 0,
    };

    FILE* file;
    file = fopen(file_name, "rb");
    if (file == NULL) {
        return buffer;
    }
    fseek(file, 0, SEEK_END);
    size_t file_size = (size_t)ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size == 0) {
        return buffer;
    }

    buffer = dynamic_array_allocate_size(sizeof(char), file_size);
    size_t read_count = fread(buffer.data, 1, file_size, file);

    if (read_count != file_size) {
        dynamic_array_free(&buffer);
        return buffer;
    }

    fclose(file);
    return buffer;
}
