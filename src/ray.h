#ifndef RAY_H
#define RAY_H

#include "util.h"

typedef struct {
    v3f pos;
    v3f dir;
} ray_t;

typedef struct {
    b32 hit;
    v3f pos, norm;
    f32 d;
} hitinfo_t;

typedef struct {
    float diffuse;
    color4f emit_color;
    color4f diffuse_color;
} material_t;

typedef struct {
    material_t material;
    v3f norm;
    f32 d;
} plane_t;

typedef struct {
    material_t material;
    v3f center;
    f32 r;
} sphere_t;

typedef struct {
    v3f pos;
    mat3f orientation;
} camera_t; 

typedef struct {
    v3f center;
    mat3f orientation;
    f32 pix_sz;
    u32 x_pix, y_pix;
    f32 x_ratio, y_ratio;
} film_t;

typedef struct {
    camera_t camera;
    film_t film;
    material_t skybox;
    u32 n_planes;
    plane_t* planes;
    u32 n_spheres;
    sphere_t* spheres;
    
} scene_t;

#endif //RAY_H
