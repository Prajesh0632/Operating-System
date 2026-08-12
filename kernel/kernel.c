#define MAX_COL 80
#define MAX_ROW 25 
#define VGA_ADDRESS 0xb8000

int row = 0, col = 0;

void kprint(char* string) {

    volatile char *video_memory = (volatile char *)VGA_ADDRESS;

    int color = 0x0F;
    for(int i = 0; string[i] != '\0'; i++) {
        
        
        if(string[i] == '\n') {
            row++;
            col = 0;
        }

        else {

        int offset = (row * MAX_COL + col) * 2;
 

        video_memory[offset] = string[i];
        video_memory[offset + 1] = color;

        col ++;

        if(col >= MAX_COL) {
            col = 0;
            row ++;
        }
            
        }
      

        if(row >= MAX_ROW) {
            row = 0;
        }
       

        
    }

    




}




void main() {
    
    char* name = "Prajesh\n";
    kprint(name);

    char* surname = "Subedi\n";
    kprint(surname);

    char* country = "Nepal";
    kprint(country);

    kprint("\nHello, World!");


    
    
    
}

void _start() {
    main();
    while(1); // Infinite halt loop
}