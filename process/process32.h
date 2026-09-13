#pragma once 
#include<stdint.h>
#include"../file_system/fat16.h"

#define USER_STACK_TOP 0xBFFFF000

 typedef struct Vma{
  // Virtual Memory Area With Linked List

    uint32_t v_start;//start of the virtual address 
    uint32_t v_end;//end of the virtual address 
    uint32_t v_flags;

    uint32_t file_offset;//if in file 

    struct Vma* next;//pointer to the next vma node 

}Vma;





typedef struct{

    uint32_t pid;  // Process ID 
    uint32_t sp;  //  Process Stack Pointer 
    uint32_t ip; //   Process Instruction Pointer 

    uint32_t hp; // Process Heap Pointer

    uint32_t* page_directory;// Process Page Directory 


    Vma* vma_list;

}Process_32;



Process_32* create_process();