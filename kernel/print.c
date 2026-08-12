#define MAX_COL 80
#define MAX_ROW 25 
#define VGA_ADDRESS 0xb8000
#define COLOR 0x0F

int row = 0, col = 0;


void scroll(char*);

void kprint(char* string) {

    volatile char *video_memory = (volatile char *)VGA_ADDRESS;

    for(int i = 0; string[i] != '\0'; i++) {
        
        
        if(string[i] == '\n') {
            row++;
            col = 0;
        }

        else {

        int offset = (row * MAX_COL + col) * 2;
 

        video_memory[offset] = string[i];
        video_memory[offset + 1] = COLOR;
        col ++;

        if(col == MAX_COL) {
            col = 0;
            row ++;
        }
            
        }
      

        if(row == MAX_ROW) {

            scroll(video_memory);
            row = MAX_ROW - 1;
        }
       

        
    }

    




}


void scroll(char* video_memory) {


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
       video_memory[offset_last + 1] = COLOR;
    }

}
