#include "core/math/vector_3.h"

#include <math.h>

vector_3f_t vector_3f_substract(vector_3f_t lhs, vector_3f_t rhs) {
    vector_3f_t result = {
      .position = {
                   .x = lhs.position.x - rhs.position.x,
                   .y = lhs.position.y - rhs.position.y,
                   .z = lhs.position.z - rhs.position.z,
                   }
    };

    return result;
}

vector_3f_t vector_3f_divide(vector_3f_t vector, float divider) {
    vector_3f_t result = {
      .position = {
                   .x = vector.position.x / divider,
                   .y = vector.position.y / divider,
                   .z = vector.position.z / divider,
                   }
    };

    return result;
}

vector_3f_t vector_3f_multiply(vector_3f_t vector, float product) {
    vector_3f_t result = {
      .position = {
                   .x = vector.position.x * product,
                   .y = vector.position.y * product,
                   .z = vector.position.z * product,
                   }
    };

    return result;
}

float vector_3f_dot_product(vector_3f_t lhs, vector_3f_t rhs) {
    return lhs.position.x * rhs.position.x + lhs.position.y * rhs.position.y
           + lhs.position.z * rhs.position.z;
}

vector_3f_t vector_3f_cross_product(vector_3f_t lhs, vector_3f_t rhs) {
    vector_3f_t result = {
      .position = {
                   .x = lhs.position.y * rhs.position.z - lhs.position.z * rhs.position.y,
                   .y = lhs.position.z * rhs.position.x - lhs.position.x * rhs.position.z,
                   .z = lhs.position.x * rhs.position.y - lhs.position.y * rhs.position.x,
                   }
    };

    return result;
}

float vector_3f_magnitude(vector_3f_t vector) {
    float value = (float)sqrt(
      pow(vector.position.x, 2) + pow(vector.position.y, 2) + pow(vector.position.z, 2));

    return value;
}

vector_3f_t vector_3f_normalise(vector_3f_t vector) {
    return vector_3f_divide(vector, vector_3f_magnitude(vector));
}
