
#include <stdint.h>

#define MEMORY_MAP_BUFFER ((mem_entry_t*)0x8000)
#define MEMORY_MAP_COUNT  (*(uint16_t*)0x77FC)

#define PAGE_SIZE 4096
#define MAX_FRAMES 262144 // for upto 1GB RAM

extern char _kernel_start[];
extern char _kernel_end[];


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

void free(uint64_t*, uint64_t);