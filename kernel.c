int offset_x = 0, offset_y = 0;

void kprint(char* string) {

    volatile char *video_memory = (volatile char *)(0xb8000 + offset_x);

    int index = 0;
    int color = 0x0F;
    for(int i = 0; string[i] != '\0'; i++) {

        video_memory[index] = string[i];
        video_memory[index + 1] = color;

        index += 2;
        
    }

    offset_x += index;




}




void main() {
    
    char* name = "Prajesh";
    kprint(name);

    char* surname = " Subedi";
    kprint(surname);

    
    
    
}

void _start() {
    main();
    while(1); // Infinite halt loop
}