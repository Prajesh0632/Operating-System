#include "elf_loader.h"
#include "elf.h"
#include "pmm.h"
#include "../file_system/fat16.h"
#include "../file_system/ata.h"
#include "../screen_driver/screen.h"
#include "../headers/string/str.h"
#include "../process/process32.h"
#include "heap.h"
#include "memory.h"
#include "paging.h"
#include "vmm.h"

/* FAT16 primitives that aren't in fat16.h (defined in fat16.c) */
extern uint32_t bytes_per_sector;
extern void dir_location(uint32_t cluster, uint32_t *lba, uint32_t *sector_count);
extern uint16_t get_next_cluster(uint16_t cluster);
extern void get_filename(char *fname, char *name);

#define ELF_MAX_SIZE (64 * 1024)



/* ------------------------------------------------------------------ */
/* inspection                                                          */
/* ------------------------------------------------------------------ */

static void line(const char *label, uint32_t value)
{
    sprint((char *)label, -1, -1);
    hprint(value);
    sprint("\n", -1, -1);
}

void load_program(const char *name, uint16_t dir_cluster, Process_32 *process)
{

    DirEntry De;

    if (!fat_find(name, dir_cluster, &De))
    {
        sprint("Cannot find the file specified.\n", -1, -1);
        return;
    }

    uint8_t *content = (uint8_t *)fralloc_kernel(De.size);
    memset(content, 0, De.size);

    if (!fat_read(&De, content, De.size))
    {
        sprint("Cannot read file.\n", -1, -1);
        return;
    }

    Elf32_Ehdr *hddr = (Elf32_Ehdr *)content;

    if (hddr->e_ident[0] != 0x7F || hddr->e_ident[1] != 'E' || hddr->e_ident[2] != 'L' || hddr->e_ident[3] != 'F')
    {
        sprint("Not an elf file.\n", -1, -1);
        return;
    }

    if (hddr->e_phnum == 0)
    {
        sprint("No Program header found.\n", -1, -1);
        return;
    }

    Elf32_Phdr *phddrs = (Elf32_Phdr *)((uint8_t *)hddr + hddr->e_phoff);
    uint32_t last_vaddr = 0;
    uint32_t process_start_vaddr = 0;

    for (uint32_t i = 0; i < hddr->e_phnum; i++)
    {

        Elf32_Phdr *phddr = &phddrs[i];

        if (phddr->p_type != PT_LOAD)
            continue;

        uint32_t vaddr = phddr->p_vaddr;
        uint32_t vaddr_aligned = align_down(vaddr);
        uint32_t mem_size = phddr->p_memsz + (vaddr - vaddr_aligned);
        uint32_t pages_required = align_up(mem_size) / PAGE_SIZE;
        process_start_vaddr = vaddr_aligned;

        Vma *new_vma = (Vma *)halloc(sizeof(Vma));
        new_vma->v_start = vaddr_aligned;
        new_vma->v_end = vaddr_aligned + PAGE_SIZE * pages_required - 1;
        new_vma->next = NULL;



        Vma* current_vma = process->vma_list;
        if (process->vma_list == NULL)
        {
            process->vma_list = new_vma;
            current_vma = process->vma_list;
        }

        else
        {

            Vma *vaddr_tail = process->vma_list;
            while (vaddr_tail->next != NULL)
                vaddr_tail = vaddr_tail->next;

            vaddr_tail->next = new_vma;
            current_vma = vaddr_tail->next;
            vaddr_tail = vaddr_tail->next;
            
        }

        for (uint32_t p = 0; p < pages_required; p++)
        {

            uint32_t cur_vaddr = vaddr_aligned + (p * PAGE_SIZE);
            vmm_map_page(cur_vaddr, process->page_directory);
        }

        uint32_t f_size = phddr->p_filesz;

        if (f_size > 0)
        {

           
            current_vma->inFile = true;
            current_vma->vaddr = vaddr_aligned;
            current_vma->segment_offset = (uint32_t)phddr->p_offset;
            current_vma->segment_size = f_size;
            current_vma->segment_index = i;
            current_vma->bss_offset = ((uint32_t)(vaddr_aligned) + f_size);
            current_vma->bss_size = phddr->p_memsz - f_size;
            current_vma->pages_required = pages_required;
            
            
        }

        uint32_t new_vaddr = vaddr_aligned + pages_required * PAGE_SIZE;
        last_vaddr = last_vaddr > new_vaddr ? last_vaddr : new_vaddr;
    }

    if (last_vaddr == 0)
        return;
    uint32_t process_heap_vaddr = align_up(last_vaddr);
    uint32_t process_stack_vaddr = align_down(USER_STACK_TOP);

    if (process_stack_vaddr <= process_heap_vaddr)
        return;

    process->sp = process_stack_vaddr;
    process->hp_start = process_heap_vaddr;
    process->hp_end = process_heap_vaddr;
    process->ip = process_start_vaddr;
    
    Vma* stack_vma = (Vma*)halloc(sizeof(Vma));
    stack_vma->v_end = process_stack_vaddr;
    stack_vma->vaddr = stack_vma->v_end;
    stack_vma->pages_required = 4;
    stack_vma->v_start = stack_vma->v_end - stack_vma->pages_required * PAGE_SIZE + 1;
    stack_vma->inFile = false;

    for(uint32_t s = 0; s < stack_vma->pages_required; s++) vmm_map_page(stack_vma->v_start - (s * PAGE_SIZE), process->page_directory);

    Vma* vma_tail = process->vma_list;
    while(vma_tail->next != NULL) vma_tail = vma_tail->next;

    vma_tail->next = stack_vma;
    stack_vma->next = NULL;


    process->ready = true;



    // content came from fralloc_kernel (phys+KERNEL_VMA); free() indexes the
    // frame bitmap by physical address, so convert back or this corrupts
    // bitmap entries far past its end.
    free((uint64_t*)(uintptr_t)virt_to_phys(content), De.size);

}
