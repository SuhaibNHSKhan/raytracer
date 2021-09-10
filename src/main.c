#include <stdio.h>
#include <malloc.h>

#include "util.c"
#include "math.c"
#include "ray.c"

int main(int argc, char** argv) {
    
#if 1
    u32 height = 720;
    u32 width  = 1280;
    
    img_t img = image_new(width, height);
    
    camera_t c = camera_new((v3f) {0.0f, 0.0f, -10.0f}, (v3f) {0.0f, 0.0f, 0.0f});
    film_t f = film_new(&c, 5.0f, width, height);
    
    scene_t scene;
    
    plane_t planes[] = {
        (plane_t) {
            .material = (material_t) { .clr = (color) { .val = 0xffbd9e31}},
            .norm = global_y,
            .d = -5.0f
        }
    };
    
    sphere_t spheres[] = {
        (sphere_t) {
            .material = (material_t) { .clr = (color) { .val = 0xff31bd91}},
            .center = (v3f) {2.0f, -5.0f, 30.0f},
            .r = 3.0f
        },
        (sphere_t) {
            .material = (material_t) { .clr = (color) { .val = 0xffac75e0}},
            .center = (v3f) {-6.0f, -4.0f, 25.0f},
            .r = 3.0f
        }
    };
    
    scene.camera = c;
    scene.film = f;
    scene.n_planes = 1;
    scene.planes = planes;
    scene.n_spheres = 2;
    scene.spheres = spheres;
    
    u32* res = img.pixels;
    
    for (u32 y = 0; y < height; ++y) {
        for (u32 x = 0; x < width; ++x) {
            v3f to = film_pixel_vec(&f, x, y);
            
            ray_t ray = ray_to(c.pos, to);
            
            *res++ = scene_cast_ray(&scene, &ray);
            
        }
    }
    
    image_write(img, fopen("data/test.bmp", "wb"));
    
#endif
}