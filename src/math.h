#ifndef MATH_H
#define MATH_H

#include "util.h"

#define global_x (v3f) { .x = 1.0f, .y = 0.0f, .z = 0.0f }
#define global_y (v3f) { .x = 0.0f, .y = 1.0f, .z = 0.0f }
#define global_z (v3f) { .x = 0.0f, .y = 0.0f, .z = 1.0f }

#define PI 3.14159265f

typedef struct {
    float x, y, z;
} v3f;

typedef struct {
    union {
        struct {
            float w, x, y, z;
        };
        struct {
            float r, g, b, a;
        };
    };
} v4f;

typedef v4f color4f;

typedef struct {
    union {
        f32 dat[3 * 3];
        v3f vec[3];
        struct {
            v3f x, y, z;
        };
    };
} mat3f;

typedef struct {
    union {
        f32 dat[4 * 4];
        struct {
            v4f vec[4];
        };
    };
} mat4f;

#endif //MATH_H
