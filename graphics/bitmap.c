#include "bitmap.h"
#include "../file_system/fat16.h"
#include "../memory/heap.h"
#include "../graphics/font_renderer.h"
#include "../graphics/vbe.h"
#include "../memory/memory.h"
#include "../memory/pmm.h"



void init_image(char* filename, uint16_t cluster) {


    DirEntry img;

    if(!fat_find(filename, cluster, &img)) {
        write_text("File not found!\n");
        return;
    }

    uint8_t *content = (uint8_t *)fralloc_kernel(img.size);
    memset(content, 0, img.size);

    if(!fat_read(&img, content, img.size)) {
        write_text("Cannot read the image!\n");
    }


    BMPFileHeader* bmp_hdr = (BMPFileHeader*)(uintptr_t)content;
    BMPInfoHeader* bmp_ihdr = (BMPInfoHeader*)(uintptr_t)(content + sizeof(BMPFileHeader));

    if(bmp_hdr->type != 0x4D42) {
        write_text("Unsupported file format.");
    }

    uint8_t* pixels = (uint8_t*)(content + bmp_hdr->offset);
    uint32_t bpp = bmp_ihdr->bpp;
    uint32_t img_height = -bmp_ihdr->height;
    uint32_t img_width = bmp_ihdr->width;

    uint32_t row_stride = img_width * 3;
    for(uint32_t i = 0; i < img_height; i++) {
        for(uint32_t j = 0; j < img_width; j++) {

            uint8_t* pixel = &pixels[i * row_stride + j * (bpp / 8)];
            Color color = {pixel[2], pixel[1], pixel[0]};
            put_pixel(j, i, color);
           
            

        }
    }


}
