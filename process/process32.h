#pragma once 
#include<stdint.h>
#include"../file_system/fat16.h"


 typedef struct{
  // Virtual Memory Area With Linked List

    uint32_t v_start;//start of the virtual address 
    uint32_t v_end;//end of the virtual address 
    uint32_t v_flags;

    uint32_t file_offset;//if in file 

    Vma* next;//pointer to the next vma node 

}Vma;





typedef struct{

    uint32_t pid;  // Process ID 
    uint32_t sp;  //  Process Stack Pointer 
    uint32_t ip; //   Process Instruction Pointer 

    uint32_t* page_directory;// Process Page Directory 


    Vma* vma_list;

}Process_32;