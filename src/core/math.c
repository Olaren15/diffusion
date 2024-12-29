#include "core/math.h"

uint32_t clamp_uint32_t(uint32_t min, uint32_t max, uint32_t value) {
    return max_uint32_t(min, min_uint32_t(max, value));
}

uint32_t max_uint32_t(uint32_t a, uint32_t b) {
    return a > b ? a : b;
}

uint32_t min_uint32_t(uint32_t a, uint32_t b) {
    return a < b ? a : b;
}
