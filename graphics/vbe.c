#include "vbe.h"
#include "../screen_driver/screen.h"
#include "../memory/paging.h"
#include "../memory/pmm.h"
#include <stddef.h>

uint8_t *framebuffer = NULL;
uint8_t bpp = 0;
uint16_t pitch = 0;

void init_graphics()
{

    vbe_mode_info_structure *display = (vbe_mode_info_structure *)VBE_INFO_ADDR;

    // display->framebuffer is a raw physical address (often near 4GB, e.g.
    // QEMU's std VGA BAR) -- it's never covered by the RAM identity map, and
    // with paging on, dereferencing it directly either faults or, worse,
    // silently aliases into ordinary RAM through the high-half mirror (see
    // init_paging()). Map it explicitly to a virtual address just past the
    // end of the identity-mapped RAM, which is guaranteed free.
    uint32_t paddr_base = align_down(display->framebuffer);
    uint32_t fb_bytes = (uint32_t)display->pitch * display->height;
    uint32_t num_pages = (fb_bytes + PAGE_SIZE - 1) / PAGE_SIZE;

    uint32_t page_directory_size = (frames + 1023) / 1024;
    uint32_t vaddr_base = page_directory_size * 0x400000;

    for (uint32_t p = 0; p < num_pages; p++)
    {
        uint32_t vaddr = vaddr_base + p * PAGE_SIZE;
        map_page_to(vaddr, paddr_base + p * PAGE_SIZE, page_directory);
        invlpg(vaddr);
    }

    framebuffer = (uint8_t *)vaddr_base;

    bpp = display->bpp / 8;

    pitch = display->pitch;

    for (uint32_t i = 0; i < WINDOW_HEIGHT; i++)
    {
        for (uint32_t j = 0; j < WINDOW_WIDTH; j++)
        {

            uint32_t x = j, y = i;
            Color c = {255, 255, 255};
            put_pixel(x, y, c);
            
        }
    }
}

void put_pixel(uint32_t x, uint32_t y, Color color)
{ 

    // if(!framebuffer || !pitch || !bpp) return;

    // pixel (x, y), assuming BGR order (typical for VBE direct-color modes)
    uint8_t *pixel = framebuffer + y * pitch + x * bpp;
    pixel[0] = color.blue;
    pixel[1] = color.green;
    pixel[2] = color.red;
}