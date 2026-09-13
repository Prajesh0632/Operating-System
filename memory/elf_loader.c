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

/* FAT16 primitives that aren't in fat16.h (defined in fat16.c) */
extern uint32_t bytes_per_sector;
extern void dir_location(uint32_t cluster, uint32_t *lba, uint32_t *sector_count);
extern uint16_t get_next_cluster(uint16_t cluster);
extern void get_filename(char *fname, char *name);

#define ELF_MAX_SIZE (64 * 1024)

/* ------------------------------------------------------------------ */
/* minimal FAT16 access, built on the primitives above                */
/* ------------------------------------------------------------------ */

static int fat_find(const char *name, uint16_t dir_cluster, DirEntry *out)
{
    uint16_t buf[256];
    uint32_t lba, count;
    uint16_t cluster = dir_cluster;
    uint32_t per_sector = bytes_per_sector / sizeof(DirEntry);

    while (cluster == 0 || (cluster >= 0x0002 && cluster < 0xFFF8))
    {

        dir_location(cluster, &lba, &count);

        for (uint32_t s = 0; s < count; s++)
        {

            ata_read_sector(lba + s, buf);
            DirEntry *e = (DirEntry *)buf;

            for (uint32_t i = 0; i < per_sector; i++)
            {

                if (e[i].name[0] == 0x00)
                    return 0; /* end of directory */
                if (e[i].name[0] == 0xE5)
                    continue; /* deleted          */
                if (e[i].attribute == ATTR_LFN)
                    continue;
                if (e[i].attribute & ATTR_VOLUME_ID)
                    continue;

                char fname[13];
                get_filename(fname, (char *)e[i].name);

                if (strcmp(fname, name) == 0)
                {
                    *out = e[i];
                    return 1;
                }
            }
        }

        if (cluster == 0)
            break;
        cluster = get_next_cluster(cluster);
    }

    return 0;
}

/* copy the whole file into dst (up to cap bytes); returns bytes written */
static uint32_t fat_read(const DirEntry *file, uint8_t *dst, uint32_t cap)
{
    uint16_t sect[256];
    uint32_t lba, count;
    uint16_t cluster = file->low_cluster;
    uint32_t remaining = file->size;
    uint32_t written = 0;

    while (remaining > 0 && cluster >= 0x0002 && cluster < 0xFFF8)
    {

        dir_location(cluster, &lba, &count);

        for (uint32_t s = 0; s < count && remaining > 0; s++)
        {

            ata_read_sector(lba + s, sect);

            uint32_t n = (remaining < bytes_per_sector) ? remaining : bytes_per_sector;
            if (written + n > cap)
                n = cap - written;

            uint8_t *src = (uint8_t *)sect;
            for (uint32_t i = 0; i < n; i++)
                dst[written + i] = src[i];

            written += n;
            remaining -= n;

            if (written >= cap)
                return written;
        }

        cluster = get_next_cluster(cluster);
    }

    return written;
}

/* ------------------------------------------------------------------ */
/* inspection                                                          */
/* ------------------------------------------------------------------ */

static void line(const char *label, uint32_t value)
{
    sprint((char *)label, -1, -1);
    hprint(value);
    sprint("\n", -1, -1);
}

void elf_inspect(const char *name, uint16_t dir_cluster, Process_32 *process)
{

    DirEntry De;

    if (!fat_find(name, dir_cluster, &De))
    {
        sprint("Cannot find the file specified.\n", -1, -1);
        return;
    }

    uint8_t *content = (uint8_t *)(uintptr_t)fralloc(De.size);
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



        if(process->vma_list == NULL) {
            process->vma_list = new_vma;
        }

        else {

            Vma *vaddr_tail = process->vma_list;
            while(vaddr_tail->next != NULL) vaddr_tail = vaddr_tail->next;

            vaddr_tail->next = new_vma;
            vaddr_tail = vaddr_tail->next;

        }

      
        for (uint32_t p = 0; p < pages_required; p++)
        {

            uint32_t cur_vaddr = vaddr_aligned + (p * PAGE_SIZE);
            map_page(cur_vaddr, process->page_directory);
        }

        void* dst = (void*)vaddr_aligned;
        void* src = (void*)((uint8_t*)hddr + phddr->p_offset);
        uint32_t f_size = phddr->p_filesz;

        if(f_size > 0) {

           
          
            memcpy(dst, src, f_size);
          
            
        }

        void* bss_dst = (void*)((uint32_t)(vaddr_aligned) + f_size);
        uint8_t value = 0;
        uint32_t bss_size = phddr->p_memsz - f_size;

        memset(bss_dst, value, bss_size);


        uint32_t new_vaddr = vaddr_aligned + pages_required * PAGE_SIZE;
        last_vaddr = last_vaddr > new_vaddr ? last_vaddr : new_vaddr;



    }

    if(last_vaddr == 0) return;
    uint32_t process_heap_vaddr = align_up(last_vaddr);
    uint32_t process_stack_vaddr = USER_STACK_TOP;

    if(process_stack_vaddr <= process_heap_vaddr) return;
    
    process->sp = process_stack_vaddr;
    process->hp = process_heap_vaddr;
    process->ip = process_start_vaddr;
    


    
}
