#include <stdio.h>
#include <malloc.h>
#include <time.h>

#include "util.c"
#include "math.c"
#include "ray.c"

#define PROGRESS

int main(int argc, char** argv) {
    
#if 1
    u32 height = 720;
    u32 width  = 1280;
    u32 samples = 4096;
    u32 depth = 32;
    f32 samplesr = 1.0f / samples;
    
    img_t img = image_new(width, height);
    
    camera_t c = camera_new((v3f) {0.0f, 0.0f, -10.0f}, (v3f) {0.0f, 0.0f, 0.0f});
    film_t f = film_new(&c, 5.0f, width, height);
    
    scene_t scene;
    
    plane_t planes[] = {
        (plane_t) {
            .material = (material_t) { .diffuse = 1.0f, .diffuse_color = color4f_newv(0xffcfaf65), .emit_color = color4f_newv(0xff000000)},
            .norm = global_y, // v3f_noz((v3f) {0.1f, 1.0f, 0.0f}),
            .d = -5.0f
        }
    };
    
    sphere_t spheres[] = {
        (sphere_t) {
            .material = (material_t) { .diffuse = 1.0f, .diffuse_color = color4f_newv(0xff57c971), .emit_color = color4f_newv(0xff000000)},
            .center = (v3f) {0.0f, -2.0f, 30.0f},
            .r = 3.0f
        },
        (sphere_t) {
            .material = (material_t) { .diffuse = 1.0f, .diffuse_color = color4f_newv(0xff000000), .emit_color = color4f_newv(0xff6577cf)},
            .center = (v3f) {6.0f, -5.0f, 30.0f},
            .r = 3.0f
        },
        (sphere_t) {
            .material = (material_t) { .diffuse = 1.0f, .diffuse_color = color4f_newv(0xff000000), .emit_color = color4f_newv(0xffb565cf)},
            .center = (v3f) {-6.0f, -5.0f, 30.0f},
            .r = 3.0f
        },
        (sphere_t) {
            .material = (material_t) { .diffuse = 1.0f, .diffuse_color = color4f_newv(0xff000000), .emit_color = color4f_newv(0xff79c2d1)},
            .center = (v3f) {0.0f, 5.0f, 30.0f},
            .r = 2.0f
        }
    };
    
    scene.camera = c;
    scene.film = f;
    scene.n_planes = 1;
    scene.planes = planes;
    scene.n_spheres = 4;
    scene.spheres = spheres;
    
#ifdef PROGRESS
    u32 total_rays = width * height * samples;
    u32 cast_rays = 0;
    u32 max_print = width * height * samples / 200;
    u32 print = max_print;
#endif
    u32* res = img.pixels;
    
    clock_t start = clock();
    
    for (u32 y = 0; y < height; ++y) {
        for (u32 x = 0; x < width; ++x) {
            
            color4f acc = color4f_def();
            
            for (u32 s = 0; s < samples; ++s) {
                v3f to = film_pixel_vec_rand(&f, x, y);
                ray_t ray = ray_to(c.pos, to);
                color4f clr = scene_cast_ray(&scene, ray, depth);
                
                color4f_accumulate(&acc, clr);
                
#ifdef PROGRESS
                cast_rays++;
                
                //printf("Casting Ray for {x: %u, y: %u}\n", x, y);
                print--;
                if (!print) printf("\rCompleted %.2f%%", cast_rays * 100.0f / total_rays);
                
                if (!print) {
                    print = max_print;
                }
#endif
            }
            
            
            
            *res++ = color4f_val(color4f_gamma_correct(color4f_mul(acc, samplesr)));
        }
    }
    
    clock_t end = clock();
    
    long elapsed = (end - start);
    
    printf("\nRaytracing complete. \nTook: %dms | %fms/bounce | %fms/ray | %fms/pixel",
           elapsed, 
           (f32) elapsed / (width * height * samples * depth),
           (f32) elapsed / (width * height * samples),
           (f32) elapsed / (width * height));
    
    image_write(img, fopen("data/test.bmp", "wb"));
    
#endif
}