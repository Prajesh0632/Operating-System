#include "font_renderer.h"
#include "font_psf1.h"
#include "vbe.h"

uint32_t x_pos = 0, y_pos = 0;

void write_text(char* text) {
 
    
    uint32_t x = x_pos;
    uint32_t y = y_pos;
    Color color = {255, 255, 255};

    for(uint32_t i = 0; text[i] != '\0'; i++) {

            

        write_c(text[i], x, y,color, 1);
        x += 8;
        if(text[i] == '\n') {
            x = 0;
            y += 20;
        }
        if(text[i] == '\b') {
            write_c(' ', x - 1, y, color, 1);
        }

       
    }

    x_pos = x;
    y_pos = y;
    if(x_pos > WINDOW_WIDTH) {
        x_pos = 0; 
        y_pos += 20;
    }

    
}

void write_c(char c, uint32_t x, uint32_t y, Color color, uint32_t fontsize) {

   

    
    
    uint32_t x_orig = x;
    uint32_t y_orig = y;
    uint32_t offset = 4 + (unsigned char)c * 16;
    uint32_t font_x = fontsize;
    uint32_t font_y = fontsize;

    for(uint32_t i = 0; i < 16; i++) {

        uint8_t pixels = font_psf1_data[offset + i];

        
        for(int j = 7; j >= 0; j--) {
            uint8_t pixel_present = (pixels >> j) & 1;
            if(pixel_present) {
              

                   uint32_t px = x_orig + (7 - j) * fontsize;
                uint32_t py = y + i * fontsize;
                for (uint32_t sy = 0; sy < fontsize; sy++)
                    for (uint32_t sx = 0; sx < fontsize; sx++)
                        put_pixel(px + sx, py + sy, color);
                
                
               }  

            }

            x = x_orig;
            y ++;
          
            
        }
    }

