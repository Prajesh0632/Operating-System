#pragma once 



#include <stdint.h>

/* =========================================================================
 * Page Directory Entry (PDE) & Page Table Entry (PTE) Flags (Bits 0 - 11)
 * ========================================================================= */

// Bit 0: Present flag (1 = Page/Table in RAM, 0 = Page fault on access)
#define PDE_PRESENT         (1 << 0)  // 0x001
#define PTE_PRESENT         (1 << 0)  // 0x001

// Bit 1: Read/Write flag (1 = Read/Write, 0 = Read-Only)
#define PDE_WRITABLE        (1 << 1)  // 0x002
#define PTE_WRITABLE        (1 << 1)  // 0x002

// Bit 2: User/Supervisor flag (1 = User Mode access, 0 = Kernel Mode only)
#define PDE_USER            (1 << 2)  // 0x004
#define PTE_USER            (1 << 2)  // 0x004

// Bit 3: Page-level Write-Through caching (1 = Write-through, 0 = Write-back)
#define PDE_WRITE_THROUGH   (1 << 3)  // 0x008
#define PTE_WRITE_THROUGH   (1 << 3)  // 0x008

// Bit 4: Page-level Cache Disable (1 = Cache disabled, 0 = Cached)
#define PDE_CACHE_DISABLE   (1 << 4)  // 0x010
#define PTE_CACHE_DISABLE   (1 << 4)  // 0x010

// Bit 5: Accessed flag (Set by CPU hardware when page/table is read/written)
#define PDE_ACCESSED        (1 << 5)  // 0x020
#define PTE_ACCESSED        (1 << 5)  // 0x020

// Bit 6: Dirty flag (PTE only - Set by CPU hardware when page is written to)
#define PTE_DIRTY           (1 << 6)  // 0x040

// Bit 7: Page Size / PSE (PDE only - 1 = 4MB Page, 0 = 4KB Page Table)
#define PDE_4MB_PAGE        (1 << 7)  // 0x080

// Bit 7: Page Attribute Table / PAT (PTE only)
#define PTE_PAT             (1 << 7)  // 0x080

// Bit 8: Global Page flag (PTE only - Prevents TLB flush on CR3 change)
#define PTE_GLOBAL          (1 << 8)  // 0x100





extern uint32_t* page_directory;
extern uint32_t* page_table;

void init_paging();
extern void loadPageDirectory(unsigned int*);
extern void enablePaging();
extern uint32_t readCR2();
void guard_frame(uint32_t*, uint32_t);

void invlpg(uint32_t);

void map_page(uint32_t, uint32_t*);

void map_page_to(uint32_t, uint32_t, uint32_t*);