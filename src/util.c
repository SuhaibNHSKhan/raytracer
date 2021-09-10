#include "util.h"
#include <malloc.h>
#include <stdio.h>

#pragma pack(push, 1)

typedef struct {
    u16 magic;
    u32 file_bytes;
    u16 reserved_1;
    u16 reserved_2;
    u32 data_offset;
    
} bmp_header_t;

typedef struct {
    u32 info_header_size;
    s32 width;
    s32 height;
    u16 planes;
    u16 bits_per_pixel;
    u32 compression;
    u32 raw_data_size;
    s32 h_res;
    s32 v_res;
    u32 color_count;
    u32 imp_color_count;
    
} bmp_info_header_t;

#pragma pack(pop)


typedef struct {
    u32 width, height;
    u32* pixels;
} img_t;


img_t image_new(u32 width, u32 height) {
    img_t img = {
        .width = width,
        .height = height,
        .pixels = (u32*) malloc(width * height * sizeof(u32))
    };
    
    return img;
}

u32 image_bytes(img_t img) {
    return img.width * img.height * sizeof(u32);
}

u32 image_write(img_t img, FILE* f) {
    if (f) {
        u32 bytes = image_bytes(img);
        
        bmp_header_t header = {
            .magic = 0x4d42,
            .file_bytes = sizeof(bmp_header_t) + bytes,
            .reserved_1 = 0,
            .reserved_2 = 0,
            .data_offset = sizeof(bmp_header_t) + sizeof(bmp_info_header_t)
        };
        
        bmp_info_header_t info_header = {
            .info_header_size = sizeof(bmp_info_header_t),
            .width = img.width,
            .height = img.height,
            .planes = 0,
            .bits_per_pixel = 32,
            .compression = 0,
            .raw_data_size = 0,
            .h_res = 1000,
            .v_res = 1000,
            .color_count = 0,
            .imp_color_count = 0
        };
        
        fwrite(&header, sizeof(bmp_header_t), 1, f);
        fwrite(&info_header, sizeof(bmp_info_header_t), 1, f);
        fwrite(img.pixels, bytes, 1, f);
        
        fclose(f);
        
        return 1;
    } else {
        printf("ERROR: NULL output stream provided\n");
        
        return 0;
    }
    
}