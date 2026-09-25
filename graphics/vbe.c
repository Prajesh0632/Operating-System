#include "vbe.h"
#include "../screen_driver/screen.h"
#include "../memory/paging.h"
#include "../memory/pmm.h"
#include "../memory/memory.h"
#include <stddef.h>

uint8_t* back_buffer = NULL;

uint8_t *framebuffer = NULL;
uint8_t bpp = 0;
uint16_t pitch = 0;
uint32_t fb_size = 0;

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
    fb_size = fb_bytes;

    

    uint32_t page_directory_size = (frames + 1023) / 1024;
    uint32_t vaddr_base = page_directory_size * 0x400000;

    for (uint32_t p = 0; p < num_pages; p++)
    {
        uint32_t vaddr = vaddr_base + p * PAGE_SIZE;
        map_page_to(vaddr, paddr_base + p * PAGE_SIZE, page_directory);
        invlpg(vaddr);
    }

    framebuffer = (uint8_t *)vaddr_base;

    // Back buffer: fresh RAM frames (not MMIO), mapped page by page into the
    // virtual range right after the framebuffer so it's contiguous.
    uint32_t back_vaddr = vaddr_base + num_pages * PAGE_SIZE;
    for (uint32_t p = 0; p < num_pages; p++)
    {
        uint64_t frame = fralloc(PAGE_SIZE);
        if (frame == (uint64_t)-1) return;

        uint32_t vaddr = back_vaddr + p * PAGE_SIZE;
        map_page_to(vaddr, (uint32_t)frame, page_directory);
        invlpg(vaddr);
    }

    back_buffer = (uint8_t *)back_vaddr;

    bpp = display->bpp / 8;

    pitch = display->pitch;

    for (uint32_t i = 0; i < WINDOW_HEIGHT; i++)
    {
        for (uint32_t j = 0; j < WINDOW_WIDTH; j++)
        {

            uint32_t x = j, y = i;
            Color c = {0, 0, 0};
            put_pixel(x, y, c);

        }
    }
}

void put_pixel(uint32_t x, uint32_t y, Color color)
{ 

    // if(!framebuffer || !pitch || !bpp) return;

    // pixel (x, y), assuming BGR order (typical for VBE direct-color modes)
    uint8_t *pixel = back_buffer + y * pitch + x * bpp;
    pixel[0] = color.blue;
    pixel[1] = color.green;
    pixel[2] = color.red;
}


void present() {
    memcpy((void*)framebuffer, (void*)back_buffer, fb_size);
}