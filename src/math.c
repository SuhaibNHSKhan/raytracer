#include "math.h"
#include <math.h>
#include <stdlib.h>



u64 lcg_rand64(lcg_rand_t* state) {
    u64 x = state->state;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
    
    state->c = 0;
    
	return state->state = x;
}

u32 lcg_rand32(lcg_rand_t* state) {
    if (state->c + 32 > 64) {
        lcg_rand64(state);
    }
    
    u32 x = (state->state >> state->c) & 0xffffffff;
    
    state->c += 32;
    
	return x;
}

u16 lcg_rand16(lcg_rand_t* state) {
    if (state->c + 16 > 64) {
        lcg_rand64(state);
    }
    
    u16 x = (state->state >> state->c) & 0xffff;
    
    state->c += 16;
    
	return x;
}

u8 lcg_rand8(lcg_rand_t* state) {
    if (state->c + 8 > 64) {
        lcg_rand64(state);
    }
    
    u8 x = (state->state >> state->c) & 0xff;
    
    state->c += 8;
    
	return x;
}


f32 randsf(lcg_rand_t* random_series, f32 max) {
    f32 r = (f32) lcg_rand16(random_series) / U16_MAX;
    
    return (r * 2.0f - 1.0f) * max;
}

f32 clamp(f32 val, f32 min, f32 max) {
    return val > max ? max : val < min ? min : val;
}


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


// TOOD*suhaibnk): this is wrong mut some how works??
#if 0
v3f v3f_transform(v3f v, mat3f m) {
    return (v3f) {
        .x = v3f_dot(v, m.x),
        .y = v3f_dot(v, m.y),
        .z = v3f_dot(v, m.z)
    };
}
#else
v3f v3f_transform(v3f v, mat3f m) {
    return (v3f) {
        .x = v3f_dot(v, (v3f) {m.x.x, m.y.x, m.z.x}),
        .y = v3f_dot(v, (v3f) {m.x.y, m.y.y, m.z.y}),
        .z = v3f_dot(v, (v3f) {m.x.z, m.y.z, m.z.z})
    };
}
#endif
v3f v3f_reflect(v3f dir, v3f norm) {
    return v3f_add(dir, v3f_scale(norm, -2.0f * v3f_dot(norm, dir)));
}

v3f v3f_unit_sphere(lcg_rand_t* random_series) {
    return v3f_noz((v3f) { randsf(random_series, 1.0f), randsf(random_series, 1.0f), randsf(random_series, 1.0f) });
}

v3f v3f_lerp(v3f v1, v3f v2, f32 t) {
    return v3f_add(v3f_scale(v1, 1.0f - t), v3f_scale(v2, t));
}

color4f color4f_def() {
    return (color4f) {
        .r = 0.0f,
        .g = 0.0f,
        .b = 0.0f,
        .a = 1.0f
    };
}

color4f color4f_new(f32 r, f32 g, f32 b, f32 a) {
    return (color4f) {
        .r = r,
        .g = g,
        .b = b,
        .a = a
    };
}

color4f color4f_newv(u32 val) {
    return (color4f) {
        .a = ((val & 0xff000000) >> 24) / 255.0f,
        .r = ((val & 0x00ff0000) >> 16) / 255.0f,
        .g = ((val & 0x0000ff00) >> 8) / 255.0f,
        .b = (val & 0x000000ff) / 255.0f
    };
}

color4f color4f_lerp(color4f c1, color4f c2, f32 t) {
    return (color4f) {
        .r = (c1.r * (1 - t) + c2.r * t),
        .g = (c1.g * (1 - t) + c2.g * t),
        .b = (c1.b * (1 - t) + c2.b * t),
        .a = (c1.a * (1 - t) + c2.a * t)
    };
}

void color4f_accumulate(color4f* self, color4f other) {
    self->r += other.r;
    self->g += other.g;
    self->b += other.b;
    //self->a += other.a;
}

void color4f_add_attenuated(color4f* self, color4f other, f32 attenuation) {
    self->r = self->r * attenuation + other.r * (1.0f - attenuation);
    self->g = self->g * attenuation + other.g * (1.0f - attenuation);
    self->b = self->b * attenuation + other.b * (1.0f - attenuation);
    //self->a += other.a;
}

color4f color4f_mul(color4f color, f32 m) {
    return (color4f) {
        .r = color.r * m,
        .g = color.g * m,
        .b = color.b * m,
        .a = color.a
    };
}

u32 color4f_val(color4f color) {
    return 
        (((u32) (clamp(color.a, 0.0f, 1.0f) * 255.0)) << 24) |
        (((u32) (clamp(color.r, 0.0f, 1.0f) * 255.0)) << 16) |
        (((u32) (clamp(color.g, 0.0f, 1.0f) * 255.0)) << 8) |
        (((u32) (clamp(color.b, 0.0f, 1.0f) * 255.0)));
}


b32 near_zero(f32 val) {
    return val <= 0.000001f && val >= -0.000001f ? 1 : 0;
}


color4f color4f_noz(color4f color) {
    f32 mag = color.r * color.r + color.b * color.b + color.g * color.g;
    
    if (near_zero(mag)) {
        return color;
    } else {
        f32 mul = 1.0f / sqrtf(mag);
        
        return color4f_mul(color, mul);
    }
}

color4f color4f_gamma_correct(color4f color) {
    return (color4f) {
        .r = sqrtf(color.r),
        .g = sqrtf(color.g),
        .b = sqrtf(color.b),
        .a = color.a
    };
}

color4f color4f_gamma_uncorrect(color4f color) {
    return (color4f) {
        .r = color.r * color.r,
        .g = color.g * color.g,
        .b = color.b * color.b,
        .a = color.a
    };
}

color4f color4f_combine_mul(color4f c1, color4f c2) {
    return (color4f) {
        .r = c1.r * c2.r,
        .g = c1.g * c2.g,
        .b = c1.b * c2.b,
        .a = c1.a * c2.a,
    };
}

color4f color4f_add(color4f c1, color4f c2) {
    return (color4f) {
        .r = c1.r + c2.r,
        .g = c1.g + c2.g,
        .b = c1.b + c2.b,
        .a = c1.a,
    };
}



void mat3f_print(mat3f m) {
    printf("%.4f %.4f %.4f\n%.4f %.4f %.4f\n%.4f %.4f %.4f", 
           m.dat[0], m.dat[1], m.dat[2],
           m.dat[3], m.dat[4], m.dat[5],
           m.dat[6], m.dat[7], m.dat[8]);
}
