#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


#define MAX_HEAPS 10


typedef struct HeapNode
{
    uint64_t base;//start address in heap
    uint64_t limit; // size in heap
    uint8_t type; // data type
    struct HeapNode* next; // pointer to next heap node
    bool occupied; // if a node is occupied or not


}__attribute__((packed)) HeapNode;

typedef struct {
         
    HeapNode* start;
    uint64_t base;
    uint64_t availabe_memory;

} __attribute__((packed)) Heap;

void init_heap(void);
void allocate_frame(void);
uint64_t halloc(uint8_t, uint64_t);
uint64_t alloc(uint8_t, uint64_t, int);
uint64_t realloc(HeapNode*,uint8_t, uint64_t);
uint8_t hfree(uint64_t);
HeapNode* merge_nodes(HeapNode*, HeapNode*);


