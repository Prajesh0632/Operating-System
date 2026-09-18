#pragma once 
#include<stdint.h>
#include"../file_system/fat16.h"

#define USER_STACK_TOP 0xBFFFF000


 typedef struct Vma{
  // Virtual Memory Area With Linked List

    uint32_t v_start;//start of the virtual address 
    uint32_t v_end;//end of the virtual address 
    uint32_t v_flags;
      
    bool inFile;//whether a segment is in file or memory

    uint32_t vaddr; // address to load in the memory
    uint32_t segment_offset; // where to read from the file 
    uint32_t segment_size; // size to read from
    uint32_t segment_index;// index for segment to read from the program header
    
    uint32_t bss_offset;
    uint32_t bss_size;

    uint32_t pages_required;

    

    struct Vma* next;//pointer to the next vma node 

}Vma;





typedef struct{

    struct Process_32* parent;
    

    uint32_t pid;  // Process ID 
    uint32_t sp;  //  Process Stack Pointer 
    uint32_t ip; //   Process Instruction Pointer 

    bool ready; // to indicate whether a process is ready or not

    uint32_t hp_start; // Process Heap Pointer
    uint32_t hp_end;

    


    uint32_t* page_directory;// Process Page Directory  
    char filename[16];
    uint32_t dir_cluster;


    Vma* vma_list;

}Process_32;


extern Process_32* current_process;

Process_32* create_process();

