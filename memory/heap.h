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


}__attribute__((aligned(16))) HeapNode;

typedef struct {
         
    HeapNode* start;
    uint64_t base;

} __attribute__((packed)) Heap;

void init_heap(void);
uint8_t allocate_frame(void);
void* halloc(uint64_t);
uint64_t alloc(uint64_t, int);
uint64_t realloc(HeapNode*, uint64_t);
uint8_t hfree(void*);
void merge_nodes(HeapNode*, HeapNode*);


