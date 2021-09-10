#include "math.h"
#include <math.h>


// STUDY(suhaibnk):
// vec3 add, sub, dot, cross: geometric context

v3f v3f_add(v3f v1, v3f v2) {
    return (v3f) {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
        .z = v1.z + v2.z
    };
}

v3f v3f_sub(v3f v1, v3f v2) {
    return (v3f) {
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
        .z = v1.z - v2.z
    };
}

v3f v3f_scale(v3f v, f32 scale) {
    return (v3f) {
        .x = v.x * scale,
        .y = v.y * scale,
        .z = v.z * scale
    };
}

f32 v3f_dot(v3f v1, v3f v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

v3f v3f_cross(v3f v1, v3f v2) {
    return (v3f) {
        .x = v1.y * v2.z - v1.z * v2.y,
        .y = v1.z * v2.x - v1.x * v2.z,
        .z = v1.x * v2.y - v1.y * v2.x
    };
}

f32 v3f_sqmag(v3f v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

f32 v3f_mag(v3f v) {
    return sqrtf(v3f_sqmag(v));
}

v3f v3f_norm(v3f v) {
    return v3f_scale(v, 1 / v3f_mag(v));
}

v3f v3f_noz(v3f v) {
    return (v.x == 0.0f && v.y == 0.0f && v.z == 0.0f) ? v : v3f_norm(v);
}

color color_newf(f32 r, f32 g, f32 b, f32 a) {
    return (color) {
        .r = (u8) (r * 255.0f),
        .g = (u8) (g * 255.0f),
        .b = (u8) (b * 255.0f),
        .a = (u8) (a * 255.0f)
    };
}

color color_newv(u32 val) {
    return (color) {
        .val = val
    };
}

color color_lerp(color c1, color c2, f32 t) {
    return (color) {
        .r = (u8) (c1.r * (1 - t) + c2.r * t),
        .g = (u8) (c1.g * (1 - t) + c2.g * t),
        .b = (u8) (c1.b * (1 - t) + c2.b * t),
        .a = (u8) (c1.a * (1 - t) + c2.a * t)
    };
}

b32 near_zero(f32 val) {
    return val <= 0.000001f && val >= -0.000001f ? 1 : 0;
}

void mat3f_print(mat3f m) {
    printf("%.4f %.4f %.4f\n%.4f %.4f %.4f\n%.4f %.4f %.4f", 
           m.dat[0], m.dat[1], m.dat[2],
           m.dat[3], m.dat[4], m.dat[5],
           m.dat[6], m.dat[7], m.dat[8]);
}


