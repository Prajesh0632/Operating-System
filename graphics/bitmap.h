#pragma once 
#include <stdint.h>


typedef struct {
    uint16_t type;             // Must be 'BM' (0x4D42)
    uint32_t size;             // File size in bytes
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;           // Offset to start of pixel data
}__attribute__((packed)) BMPFileHeader;

typedef struct {
    uint32_t size;             // Header size
    int32_t  width;            // Image width in pixels
    int32_t  height;           // Image height in pixels (if positive, bottom-up format)
    uint16_t planes;           // Must be 1
    uint16_t bpp;              // Bits per pixel (24 for RGB, 32 for RGBA)
    uint32_t compression;      // 0 = BI_RGB (uncompressed)
    uint32_t image_size;
    int32_t  x_pixels_per_m;
    int32_t  y_pixels_per_m;
    uint32_t colors_used;
    uint32_t colors_important;
}__attribute__((packed)) BMPInfoHeader;

void init_image(char*, uint16_t);