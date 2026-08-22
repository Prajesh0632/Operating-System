static inline void sys_write(char c) {
    asm volatile("int $0x80" :: "a"(0), "b"(c));
}


void user_prog() {

    sys_write('A');
    while(1);
}