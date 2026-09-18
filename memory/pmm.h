
#include <stdint.h>

#define MEMORY_MAP_BUFFER ((mem_entry_t*)0x8000)
#define MEMORY_MAP_COUNT  (*(uint16_t*)0x77FC)

#define PAGE_SIZE 4096
#define MAX_FRAMES (262144) // for upto 4GB RAM

#define KERNEL_VMA 0xC0000000 // must match linker.ld's KERNEL_VMA

// fralloc_kernel() hands back phys+KERNEL_VMA so the kernel can dereference
// it directly. CR3/PDE/PTE content is hardware-defined to be physical, so
// convert back with this at that boundary -- never load a fralloc_kernel()
// pointer into CR3 or a table entry as-is.
static inline uint32_t virt_to_phys(void* vaddr) {
    return (uint32_t)vaddr - KERNEL_VMA;
}

// Inverse: a physical address already sitting inside a PDE/PTE (not fresh
// from fralloc) needs this before the kernel can dereference it as a
// pointer -- e.g. reading an existing page table's physical address back
// out of a directory entry.
static inline void* phys_to_virt(uint32_t paddr) {
    return (void*)(paddr + KERNEL_VMA);
}

extern char _kernel_start[];       // virtual (0xC00xxxxx)
extern char _kernel_end[];         // virtual (0xC00xxxxx)
extern char _kernel_phys_start[];  // physical load address
extern char _kernel_phys_end[];    // physical, one past the kernel image


extern uint32_t frames;
extern uint8_t bitmap[MAX_FRAMES];

 typedef struct {
      uint64_t base;
      uint64_t length;
      uint32_t type;   // 1 = usable RAM, everything else = reserved/unusable
  } __attribute__((packed)) mem_entry_t;





uint64_t align_up(uint64_t);
uint64_t align_down(uint64_t);


void init_mem(void);

uint64_t fralloc(uint64_t);
void* fralloc_kernel(uint64_t);

void free(uint64_t*, uint64_t);