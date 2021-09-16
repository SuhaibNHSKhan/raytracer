#include <stdio.h>
#include <malloc.h>
#include <time.h>

#include "util.c"
#include "math.c"
#include "thread.c"
#include "ray.c"

#define PROGRESS

int main(int argc, char** argv) {
    
#if 1
    u32 height = 720;
    u32 width  = 1280;
    u32 samples = 4096;
    u32 bounces = 32;
    
    raytrace_config_t config = (raytrace_config_t) {
        .samples = samples,
        .bounces = bounces
    };
    
    img_t img = image_new(width, height);
    
    camera_t c = camera_new((v3f) {0.0f, 0.0f, -10.0f}, (v3f) {0.0f, 0.0f, 0.0f});
    film_t f = film_new(&c, 5.0f, width, height);
    
    scene_t scene;
    
    plane_t planes[] = {
        (plane_t) {
            .material = (material_t) { .diffuse = 1.0f, .diffuse_color = color4f_newv(0xff222222), .emit_color = color4f_newv(0xff000000)},
            .norm = global_y, // v3f_noz((v3f) {0.1f, 1.0f, 0.0f}),
            .d = -5.0f
        },
        (plane_t) {
            .material = (material_t) { .diffuse = 1.0f, .diffuse_color = color4f_newv(0xff000000), .emit_color = color4f_newv(0xffffffff)},
            .norm = v3f_noz((v3f){-1.0f, -1.0f, -1.0f}), // v3f_noz((v3f) {0.1f, 1.0f, 0.0f}),
            .d = 100.0f
        }
    };
    
    sphere_t spheres[] = {
        (sphere_t) {
            .material = (material_t) { .diffuse = 0.2f, .diffuse_color = color4f_newv(0xffffffff), .emit_color = color4f_newv(0xff000000)},
            .center = (v3f) {0.0f, -2.0f, 30.0f},
            .r = 3.0f
        },
        (sphere_t) {
            .material = (material_t) { .diffuse = 1.0f, .diffuse_color = color4f_newv(0xff000000), .emit_color = color4f_newv(0xffffffff)},
            .center = (v3f) {6.0f, 0.0f, 25.0f},
            .r = 1.0f
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
    scene.n_spheres = 2;
    scene.spheres = spheres;
    
    clock_t start = clock();
    
    //u32 min_d = min(img.width, img.height);
    //u32 max_d = max(img.width, img.height);
    
    
#define CORES 8
    
    u32 block_w = 128; // width;
    u32 block_h = 128; // (height + CORES - 1) / CORES;
    
    threadpool_t threadpool = threadpool_new(CORES);
    
    progress_t progress = {
        .current_bounces = 0,
        .total_bounces = (s64) img.width * img.height * config.samples * config.bounces
    };
    
#ifdef PROGRESS
    progress_workunit_t progress_workunit;
    
    progress_workunit.progress = &progress;
    progress_workunit.threadpool = &threadpool;
    
    thread_t progress_thread = thread_run(progress_reporter, (void*) &progress_workunit);
#endif
    
    for (u32 y = 0; y < height; y += block_h) {
        for (u32 x = 0; x < width; x += block_w) {
            rt_workunit_t* work_unit = malloc(sizeof(rt_workunit_t));
            
            work_unit->min_x = x;
            work_unit->min_y = y;
            work_unit->mp1_x = min(x + block_w, img.width);
            work_unit->mp1_y = min(y + block_h, img.height);
            work_unit->config = &config;
            work_unit->scene = &scene;
            work_unit->img = &img;
            work_unit->progress = &progress;
            work_unit->random_series = (lcg_rand_t) { .state = rand(), .c = 0 };
            
            threadpool_submit(&threadpool, render_block_worker, (void*) work_unit);
        }
    }
    
    
    threadpool_end(&threadpool);
#ifdef PROGRESS
    thread_wait(&progress_thread, INFINITE);
#endif
    /*
    dword res;
    
    // TODO(suhaibnk): Wait timeout is windows specific
    while ((res = thread_wait_for_all(CORES, threads, 1, 1000)) == WAIT_TIMEOUT) {
        printf("\rRaycasting %.2f%% complete...", 100.0f * progress.current_bounces / progress.total_bounces);
    }
    */
    clock_t end = clock();
    
    long elapsed = (end - start);
    
    printf("\nRaytracing complete. \nTook: %dms",
           elapsed);
    
#if 0
    printf("\nRaytracing complete. \nTook: %dms | %fms/bounce | %fms/ray | %fms/pixel",
           elapsed, 
           (f32) elapsed / (width * height * samples * depth),
           (f32) elapsed / (width * height * samples),
           (f32) elapsed / (width * height));
#endif
    image_write(img, fopen("data/test.bmp", "wb"));
    
#endif
}