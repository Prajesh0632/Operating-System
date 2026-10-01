#include "window.h"
#include "../process/process_manager.h"
#include "../memory/pmm.h"
#include "vbe.h"
#include "../memory/memory.h"

bool create_window(uint32_t width, uint32_t height, int x_pos, int y_pos, uint32_t x_offset, uint32_t y_offset) {

    running_process->window = (Window*)process_halloc(sizeof(Window));
    Window* win = running_process->window;
    win->win_x_pos = x_pos;
    win->win_y_pos = y_pos;
    win->x_offset = x_offset;
    win->y_offset = y_offset;
    win->x_pos = win->win_x_pos + win->x_offset;
    win->y_pos = win->win_y_pos + win->y_offset;
    win->width = width;
    win->height = height;



    draw_win();
    present_process(running_process->back_buffer);
    return true;
    
}


void draw_win() {

    Process_32* process = running_process;
    Window* window = process->window;

    
    for(uint32_t i = 0; i < window->height + window->y_offset; i++) {
            for(uint32_t j = 0; j < window->width + window->x_offset; j++) {

                put_pixel(window->win_x_pos + j, window->win_y_pos + i, COLOR_WHITE);

            }


    }
   

    

    for(uint32_t i = 0; i < window->y_offset; i++) {
            for(uint32_t j = 0; j < window->width + window->x_offset; j++) {

                put_pixel(window->win_x_pos + j, window->win_y_pos + i, COLOR_RED);

            }


    }

    for(uint32_t i = 0; i < window->height + window->y_offset; i++) {
        for(uint32_t j = 0; j < window->x_offset; j++) {
            put_pixel(window->win_x_pos + j, window->win_y_pos + i, COLOR_RED);
            put_pixel(window->win_x_pos + window->width + j + window->x_offset, window->win_y_pos + i, COLOR_RED);


        }
    }

    for(uint32_t i = 0; i < window->y_offset; i++) {
            for(uint32_t j = 0; j < window->width + 2*window->x_offset; j++) {

                put_pixel(window->win_x_pos + j, window->win_y_pos + window->height + i + window->y_offset, COLOR_RED);

            }


    }
}




void update_window(uint8_t* buffer) {

    

    Process_32* process = running_process;
    Window* window = process->window;

    uint8_t* dest_row = process->back_buffer + (window->x_pos * bpp + window->y_pos * pitch);
    uint8_t* src_row = buffer;
    uint32_t src_row_bytes = window->width * bpp;

    for (uint32_t row = 0; row < window->height; row++) {
        memcpy((void*)dest_row, (void*)src_row, src_row_bytes);
        dest_row += pitch;
        src_row += src_row_bytes;
    }

    present_process(process->back_buffer);
}