#include "ray.h"

#include <assert.h>

camera_t camera_new(v3f pos, v3f lookat) {
    
    camera_t c;
    
    c.pos = pos;
    c.orientation.z = v3f_noz(v3f_sub(lookat, pos));
    c.orientation.x = v3f_noz(v3f_cross(global_y, c.orientation.z));
    c.orientation.y = v3f_noz(v3f_cross(c.orientation.z, c.orientation.x));
    
    return c;
}

film_t film_new(const camera_t* camera, f32 d, u32 x_pix, u32 y_pix) {
    assert(camera != NULL);
    
    film_t film;
    
    film.center = v3f_add(camera->pos, v3f_scale(camera->orientation.z, d));
    film.orientation = camera->orientation;
    film.x_pix = x_pix;
    film.y_pix = y_pix;
    
    film.y_ratio = 1.0f;
    film.x_ratio = film.y_ratio * x_pix / y_pix;
    
    return film;
}

v3f film_pixel_vec(const film_t* film, u32 x, u32 y) {
    f32 x_norm = 2.0f * x / film->x_pix - 1.0f;
    f32 y_norm = 2.0f * y / film->y_pix - 1.0f;
    
    f32 x_off = x_norm * film->x_ratio;
    f32 y_off = y_norm * film->y_ratio;
    
    // NOTE(suhaibnk): can be replace by the mat multiple of orientation * v3f(x_off, y_off, 0)
    // logically multiplying by orientation matrix converts from local coordination system to global
    // this would mean that we want a local transformation for v3f(x_off, y_off, 0) which then we convert to the global offset
    
    v3f off = v3f_add(v3f_scale(film->orientation.x, x_off), v3f_scale(film->orientation.y, y_off));
    
    return v3f_add(film->center, off);
    
}

ray_t ray_new(v3f pos, v3f dir) {
    return (ray_t) {
        .pos = pos,
        .dir = dir
    };
}

ray_t ray_to(v3f at, v3f to) {
    return (ray_t) {
        .pos = at,
        .dir = v3f_noz(v3f_sub(to, at))
    };
}

v3f ray_at(const ray_t* ray, f32 t) {
    return v3f_add(ray->pos, v3f_scale(ray->dir, t));
}

// NOTE(suhaibnk): Not great, assumes y component of ray @ 1 is always between -1.0 to 1.0 (true for now)
u32 skybox_hit(const ray_t* ray) {
    return color_lerp(color_newv(0xff3285a8), color_newv(0xffffffff), (ray_at(ray, 1.0f).y + 1.0f) / 2.0f).val;
}

hitinfo_t plane_rayhit(const plane_t* plane, const ray_t* ray) {
    hitinfo_t info;
    info.hit = 0;
    
    f32 den = v3f_dot(plane->norm, ray->dir);
    
    if (!near_zero(den)) {
        f32 d = (plane->d - v3f_dot(plane->norm, ray->pos)) / den;
        
        if (d > 0.0f) {
            info.hit = 1;
            info.pos = ray_at(ray, d);
            info.norm = plane->norm;
            info.d = d;
        }
    }
    
    return info;
}

hitinfo_t sphere_rayhit(const sphere_t* sphere, const ray_t* ray) {
    hitinfo_t info;
    info.hit = 0;
    
    v3f cen = v3f_sub(ray->pos, sphere->center);
    
    f32 a = v3f_dot(ray->dir, ray->dir);
    f32 b = 2.0f * v3f_dot(cen, ray->dir);
    f32 c = v3f_dot(cen, cen) - sphere->r * sphere->r;
    
    f32 sq = b * b - 4.0f * a * c;
    
    if (!near_zero(a) && sq >= 0.0f) {
        f32 d1 = (-b + sqrtf(sq)) / (2.0f * a);
        f32 d2 = (-b - sqrtf(sq)) / (2.0f * a);
        
        if (d1 > 0.0f && d1 <= d2) {
            info.hit = 1;
            info.pos = ray_at(ray, d1);
            info.norm = v3f_noz(v3f_sub(info.pos, sphere->center));
            info.d = d1;
            
        } else if (d2 > 0.0f && d2 <= d1) {
            info.hit = 1;
            info.pos = ray_at(ray, d2);
            info.norm = v3f_noz(v3f_sub(info.pos, sphere->center));
            info.d = d2;
        }
        
    }
    
    return info;
}

u32 scene_cast_ray(const scene_t* scene, const ray_t* ray) {
    u32 clr = skybox_hit(ray);
    f32 d = FLT_MAX;
    
    for (u32 i = 0; i < scene->n_planes; ++i) {
        plane_t plane = scene->planes[i];
        hitinfo_t info = plane_rayhit(&plane, ray);
        
        if (info.hit && info.d < d) {
            d = info.d;
            clr = plane.material.clr.val;
        }
    }
    
    for (u32 i = 0; i < scene->n_spheres; ++i) {
        sphere_t sphere = scene->spheres[i];
        hitinfo_t info = sphere_rayhit(&sphere, ray);
        
        if (info.hit && info.d < d) {
            d = info.d;
            clr = sphere.material.clr.val;
        }
    }
    
    
    return clr;
}