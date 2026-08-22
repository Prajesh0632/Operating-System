#include "pmm.h"
#include "../screen_driver/screen.h"


uint32_t frames = 0;
uint8_t bitmap[MAX_FRAMES]; // 1 = used, 0 = free

extern char _kernel_start[];
extern char _kernel_end[];


uint64_t align_down(uint64_t addr) {

    return addr & ~(PAGE_SIZE - 1);
}

uint64_t align_up(uint64_t addr) {
    return (addr + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1); 
}



void init_bitmap() {

    

    uint32_t total = (frames < MAX_FRAMES) ? frames : MAX_FRAMES;


    //first mark all the physical memory as used 
    for(uint32_t i = 0; i < total; i++) {

        bitmap[i] = 1;

    }




    //free frames inside the usable region 
    for(uint32_t i = 0; i < MEMORY_MAP_COUNT; i++) {
        if(MEMORY_MAP_BUFFER[i].type != 1) continue;
        
        uint64_t start = align_up(MEMORY_MAP_BUFFER[i].base);
        uint64_t end = align_down(MEMORY_MAP_BUFFER[i].base + MEMORY_MAP_BUFFER[i].length);

        for(uint64_t offset = start; offset < end; offset += PAGE_SIZE) {
            bitmap[offset / PAGE_SIZE] = 0;
        }

    }

    
    //mark for BOOTLOADER, GDT, TSS
    uint32_t start = 0;
    uint32_t end = 0x10000;
    for(uint32_t i = start; i < align_down(end); i += PAGE_SIZE) {
        bitmap[i / PAGE_SIZE] = 1;
    }


     



    //mark the space occupied by kernel 
    uint32_t k_start = (uint32_t)_kernel_start;
    uint32_t k_end = (uint32_t)_kernel_end;
    for(uint32_t start = align_down(k_start); start < align_up(k_end); start += PAGE_SIZE) {
        bitmap[start / PAGE_SIZE] = 1;
    }


    //mark the space occupied by the stack (stack starts at)
    uint32_t stack_start = (uint32_t)_kernel_end;
    uint32_t stack_end = stack_start + 0x200000;   
    for(uint32_t i = align_down(stack_start); i < align_up(stack_end); i += PAGE_SIZE) {
        bitmap[i / PAGE_SIZE] = 1;
    }


    //also mark 0x0 address as used 
    bitmap[0] = 1;

  
}

void set_bitmap_size() {
    uint64_t max_memory_offset = 0;

    for (uint32_t i = 0; i < MEMORY_MAP_COUNT; i++) {
          if(MEMORY_MAP_BUFFER[i].type != 1) continue;
          uint64_t end = MEMORY_MAP_BUFFER[i].base + MEMORY_MAP_BUFFER[i].length;

          if (end > max_memory_offset) {
             max_memory_offset = end;
          }
      }

      frames = (uint32_t)(max_memory_offset / PAGE_SIZE);
      if(frames > MAX_FRAMES) frames = MAX_FRAMES;
        

}

void init_mem() {

set_bitmap_size();
init_bitmap();



    
}


void flag_pages(int start, int end, int flag) {
    for(int i = start; i <= end; i++) {
        bitmap[i] = flag;
    }
}


 
uint64_t fralloc(uint64_t size) {

       if(size == 0) return -1;
       
       int frames_need = (size + PAGE_SIZE - 1) / PAGE_SIZE;
       int frames_found = 0;
       int start;
       

       for(int i = 0; i < frames; i++) {

        if(bitmap[i] == 0) {
          if(frames_found == 0) start = i;  
          frames_found++;
          
        }

        if(frames_found == frames_need) {
              flag_pages(start, i, 1);
              return (uint64_t)(start * PAGE_SIZE);
        }

        if(bitmap[i] == 1) {
            frames_found = 0;
        }
       }


       return -1;
     
}


void free(uint64_t* ptr, uint64_t size) {

    if(!ptr || size == 0) return;

    int frames = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    uint64_t addr = (uint64_t)(uintptr_t)(ptr);
    int start_frame = addr / PAGE_SIZE;
    int end_frame = start_frame + frames - 1;
    
    flag_pages(start_frame, end_frame, 0);

    

} 