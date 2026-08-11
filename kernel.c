void main() {
    // Offset by 160 bytes (Row 2, Column 0)
    volatile char *video_memory = (volatile char *)(0xb8000);
    
    video_memory[0] = 'Z';   // ASCII character
    video_memory[1] = 0x0F;  // Color Attribute: White on Black
}

void _start() {
    main();
    while(1); // Infinite halt loop
}