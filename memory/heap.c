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
    
    for(int i = 0; i < current_heaps; i++) {
        
        uint64_t available = PAGE_SIZE;
        HeapNode* node = heap_list[i].start;

        while(node != NULL) {

            if(node->limit >= size && !node->occupied) {
                return realloc(node, type, size);
            }
            
            available -= (node->limit + sizeof(HeapNode));
            node = node->next;
            

        }

        if(available >= size + sizeof(HeapNode)) {
           return alloc(type, size, i);
        }

        
    }

    allocate_frame();
    return alloc(type, size, current_heaps-1);

   

}



HeapNode* create_node(uint64_t addr, uint64_t limit, uint8_t type) {

    HeapNode* new_node = (HeapNode*)(uintptr_t)(addr);
    new_node->base = addr;
    new_node->limit = limit;
    new_node->type = type;
    new_node->occupied = true;
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
        heap->availabe_memory -= sizeof(HeapNode) + size;

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
    heap->availabe_memory -= sizeof(HeapNode) + size;


    return (new_node->base + sizeof(HeapNode));


}


uint64_t realloc(HeapNode* node, uint8_t type, uint64_t size) {

       
    if(node->limit == size) {

    node->type = type;
    node->occupied = true;

    return (node->base + sizeof(HeapNode));


    }

    HeapNode* next = node->next;

    uint64_t remaining = node->limit - size;

    HeapNode* left_node;


    if(remaining >= sizeof(HeapNode) + 8) {
         
        left_node = create_node(node->base, size, type);
        HeapNode* right_node = create_node(left_node->base + left_node->limit + sizeof(HeapNode), remaining - sizeof(HeapNode), type);

        left_node->next = right_node;
        right_node->next = next;
        right_node->occupied = false;

        
    }

    else {
        left_node = create_node(node->base, node->limit, type);
        left_node->next = next;
    }

    return (left_node->base + sizeof(HeapNode));


    

   

}




HeapNode* merge_nodes(HeapNode* left, HeapNode* right) {

    left->limit = left->limit + sizeof(HeapNode) + right->limit;
    left->next = right->next;
    left->occupied = 0;

    return left;

}



uint8_t hfree(uint64_t address) {
     
    int index = -1;

    for(int i = 0; i < current_heaps; i++) {

        if(address >= heap_list[i].base && address < heap_list[i].base + PAGE_SIZE) {
            index = i;
            break;
        }
    }
    
    if(index == -1) return 0;

    Heap* heap = &heap_list[index];

    HeapNode* node = heap->start;
    HeapNode* prev = NULL;

    while(node != NULL && (node->base + sizeof(HeapNode) != address)){
        
        prev = node;
        node = node->next;
    } 

    if(node == NULL) return 0;

    node->occupied = false;


    HeapNode* next = node->next;
    HeapNode* temp = NULL;

    if (next && !next->occupied) {
        temp = merge_nodes(node, next); // Merge with NEXT first (if free)

    }

    if (prev && !prev->occupied) {
        temp = merge_nodes(prev, temp);     // Merge with PREV second (if free)

        
        // If prev was the first node in the heap, heap->start remains prev!
        if (heap->start == node) {
            heap->start = prev;
        }
    }

   


    return 1;





}