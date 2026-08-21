#include "heap.h"
#include "pmm.h"




int current_heaps = 0; // current number of heaps
Heap heap_list[MAX_HEAPS]; 




uint8_t allocate_frame() {

    if(current_heaps < MAX_HEAPS)
    {

         heap_list[current_heaps].base = fralloc(PAGE_SIZE);   
         if(heap_list[current_heaps].base == -1) {
            return 0;
         } 
         heap_list[current_heaps].start = NULL;
         current_heaps++;

         return 1;
         

    }

    return 0;

    

   



}


void init_heap() {

   allocate_frame();


}


void* halloc(uint64_t size) {


    if(size + sizeof(HeapNode) > PAGE_SIZE) {
        return NULL;
    }
    
    for(int i = 0; i < current_heaps; i++) {
        
        HeapNode* node = heap_list[i].start;
        HeapNode* tail = NULL;

        while(node != NULL) {

            if(node->limit >= size && !node->occupied) {
                return (void*)(uintptr_t)realloc(node, size);
            }
            
            tail = node;
            node = node->next;
            

        }


        if(!tail) {
            return (void*)(uintptr_t)alloc(size, i);
        }
        

        uint64_t tail_end = tail->base + tail->limit + sizeof(HeapNode);

        uint64_t occupied = tail_end - heap_list[i].base;
        uint64_t available = PAGE_SIZE - occupied;

        if(available >= size + sizeof(HeapNode)) {
            return (void*)(uintptr_t)alloc(size, i);
        }
        
        
        

        
    }

   

    uint8_t success = allocate_frame();
    if(success) return (void*)(uintptr_t)alloc(size, current_heaps-1);
    
    return NULL;

    


    
   

}



HeapNode* create_node(uint64_t addr, uint64_t limit) {

    HeapNode* new_node = (HeapNode*)(uintptr_t)(addr);
    new_node->base = addr;
    new_node->limit = limit;
    new_node->occupied = true;
    new_node->next = NULL;

    return new_node;

}


uint64_t alloc(uint64_t size, int index) {
      
    Heap* heap = &heap_list[index];
    HeapNode* node = heap->start;

    if(node == NULL) {
        
        HeapNode* first_node = create_node(heap->base, size);
        heap->start = first_node;

        return (first_node->base + sizeof(HeapNode)); 
        
        

    }

    HeapNode* prev = NULL;
    while(node != NULL) {
         
        prev = node;
        node = node->next;

    }


    uint64_t new_node_addr = prev->base + sizeof(HeapNode) + prev->limit;
    HeapNode* new_node = create_node(new_node_addr, size);
    prev->next = new_node;


    return (new_node->base + sizeof(HeapNode));


}


uint64_t realloc(HeapNode* node, uint64_t size) {

       
    if(node->limit == size) {

    node->occupied = true;

    return (node->base + sizeof(HeapNode));


    }

    HeapNode* next = node->next;

    uint64_t remaining = node->limit - size;

    HeapNode* left_node;


    if(remaining >= sizeof(HeapNode) + 8) {
         
        left_node = node;
        left_node->limit = size;
        left_node->occupied = true;
        HeapNode* right_node = create_node(left_node->base + left_node->limit + sizeof(HeapNode), remaining - sizeof(HeapNode));

        left_node->next = right_node;
        right_node->next = next;
        right_node->occupied = false;

        
    }

    else {
        left_node = node;
        left_node->occupied = true;
        left_node->next = next;
    }

    return (left_node->base + sizeof(HeapNode));


    

   

}




void merge_nodes(HeapNode* left, HeapNode* right) {

    left->limit = left->limit + sizeof(HeapNode) + right->limit;
    left->next = right->next;
    left->occupied = 0;

    

}



uint8_t hfree(void* ptr) {

    uint64_t address = (uint64_t)(uintptr_t)(ptr);
     
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

    if (next && !next->occupied) {
        merge_nodes(node, next); // Merge with NEXT first (if free)

    }

    if (prev && !prev->occupied) {
        merge_nodes(prev, node);     // Merge with PREV second (if free)

        
        // If prev was the first node in the heap, heap->start remains prev!
        if (heap->start == node) {
            heap->start = prev;
        }
    }

    
     

    return 1;





}