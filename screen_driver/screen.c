#include "../port_IO/io.h"
#include "screen.h"


void init_screen() {
    cls();
    set_cursor_offset(0,0);

}



void write_char(volatile char* video_memory, int offset, char c, int color) {

    video_memory[offset] = c;
    video_memory[offset + 1] = color;

}



void sprint(char* string, int row, int col) {
    


    if(row >= 0 && col >= 0) {
        set_cursor_offset(row, col);

    }

    int offset = get_cursor_offset();
    row = offset / MAX_COL;
    col = offset % MAX_COL;
    
    


    volatile char *video_memory = (volatile char *)VGA_ADDRESS;

    int color;
    char toWrite;

    for(int i = 0; string[i] != '\0'; i++) {
        
        
        if(string[i] == '\n') {
            row++;
            col = 0;
        }

        else {


        if(string[i] == '\b'){
            if(col != 0) {
                col--;
                write_char(video_memory, get_offset(row, col), ' ', WHITE);

            }

            if(col == 0 && row != 0) {
                row--;
                col = MAX_COL-1;
            }
           

        }

        else{

            write_char(video_memory, get_offset(row, col), string[i], WHITE);
             col ++;

        if(col == MAX_COL) {
            col = 0;
            row ++;
        }
            
        }
      

       
       

            
        }


 
       
         if(row == MAX_ROW) {

            handle_scroll(video_memory);
            row = MAX_ROW - 1;
        }
        
       
        
    }


    set_cursor_offset(row, col);

    




}



int get_offset(int row_val,int col_val){

   return (row_val * MAX_COL + col_val) * 2;

}


void handle_scroll(volatile char* video_memory) {


    for(int temp_row = 0; temp_row < MAX_ROW - 1; temp_row ++) {
         
        for(int temp_col = 0; temp_col < MAX_COL; temp_col ++) {


            int offset_dst = (temp_row * MAX_COL + temp_col) * 2;
            int offset_src = ((temp_row + 1) * MAX_COL + temp_col) * 2;

            video_memory[offset_dst] = video_memory[offset_src];
            video_memory[offset_dst + 1] = video_memory[offset_src + 1];
        }
    }

    
    for(int c = 0; c < MAX_COL; c++) {
       
       int offset_last = ((MAX_ROW - 1) * MAX_COL + c) * 2;

       video_memory[offset_last] = ' ';
       video_memory[offset_last + 1] = WHITE;
    }

}

void cls() {

    int offset = get_cursor_offset();
    int row = offset / MAX_COL;
    int col = offset % MAX_COL;

    volatile char *video_memory = (volatile char *)VGA_ADDRESS;

    for(int r = 0; r < MAX_ROW; r++) {
        for(int c = 0; c < MAX_COL; c++) {
            int offset = (r * MAX_COL + c) * 2;
            video_memory[offset] = ' ';
            video_memory[offset + 1] = WHITE;
        }
    }

    row = 0;
    col = 0;

    set_cursor_offset(row, col);

}

void set_cursor_offset(int r, int c) {
    
    uint16_t offset = (r * MAX_COL + c);
    port_byte_out(INDEX_PORT, 14);
    port_byte_out(DATA_PORT, (uint8_t)(offset >> 8));

    port_byte_out(INDEX_PORT, 15);
    port_byte_out(DATA_PORT, (uint8_t)(offset & 0x00FF));

}

int get_cursor_offset() {

   uint16_t offset; 

   port_byte_out(INDEX_PORT, 14);
   offset = (uint16_t)(port_byte_in(DATA_PORT) << 8);

   port_byte_out(INDEX_PORT, 15);
   offset += (uint16_t)(port_byte_in(DATA_PORT));

   return (int)offset;




}



