#include "heap.h"
#include "pmm.h"



int current_heaps = 0; // current number of heaps
Heap heap_list[MAX_HEAPS]; 




void allocate_frame() {

    if(current_heaps < MAX_HEAPS)
    {

         heap_list[current_heaps].base = fralloc(PAGE_SIZE);    
         heap_list[current_heaps].availabe_memory = PAGE_SIZE;
         current_heaps++;
         

    }

    if(current_heaps == 1) {
        heap_list[0].start = NULL;
        
    }

   



}


void init_heap() {

   allocate_frame();


}


uint64_t halloc(uint8_t type, uint64_t size) {
    
    int index = -1;// index for available heap
    for(int i = 0; i < current_heaps; i++) {
        
        if(heap_list[i].availabe_memory >= size) {
            index = i;
            break;
        }
        
    }

    if(index == -1) {

        allocate_frame();
        index = current_heaps - 1;

    }

    return alloc(type, size, index);

}



HeapNode* create_node(uint64_t addr, uint64_t limit, uint8_t type) {

    HeapNode* new_node = (HeapNode*)(uintptr_t)(addr);
    new_node->base = addr;
    new_node->limit = limit;
    new_node->type = type;
    new_node->occupied = 1;
    new_node->next = NULL;

    return new_node;

}


uint64_t alloc(uint8_t type, uint64_t size, int index) {
      
    Heap* heap = &heap_list[index];
    HeapNode* node = heap->start;
    uint64_t offset = 0;

    if(node == NULL) {
        
        HeapNode* first_node = create_node(heap->base, size, type);
        heap->start = first_node;

        return (first_node->base + sizeof(HeapNode)); 
        
        

    }

    HeapNode* prev = NULL;
    while(node != NULL) {
         
        offset += node->limit + sizeof(HeapNode);
        prev = node;
        node = node->next;

    }



    HeapNode* new_node = create_node(heap->base + offset, size, type);

    prev->next = new_node;



    return (new_node->base + sizeof(HeapNode));


}