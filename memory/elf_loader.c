#include "elf_loader.h"
#include "elf.h"
#include "pmm.h"
#include "../file_system/fat16.h"
#include "../file_system/ata.h"
#include "../screen_driver/screen.h"
#include "../headers/string/str.h"

/* FAT16 primitives that aren't in fat16.h (defined in fat16.c) */
extern uint32_t bytes_per_sector;
extern void     dir_location(uint32_t cluster, uint32_t *lba, uint32_t *sector_count);
extern uint16_t get_next_cluster(uint16_t cluster);
extern void     get_filename(char *fname, char *name);

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

    while (cluster == 0 || (cluster >= 0x0002 && cluster < 0xFFF8)) {

        dir_location(cluster, &lba, &count);

        for (uint32_t s = 0; s < count; s++) {

            ata_read_sector(lba + s, buf);
            DirEntry *e = (DirEntry *)buf;

            for (uint32_t i = 0; i < per_sector; i++) {

                if (e[i].name[0] == 0x00)  return 0;   /* end of directory */
                if (e[i].name[0] == 0xE5)  continue;   /* deleted          */
                if (e[i].attribute == ATTR_LFN)       continue;
                if (e[i].attribute & ATTR_VOLUME_ID)  continue;

                char fname[13];
                get_filename(fname, (char *)e[i].name);

                if (strcmp(fname, name) == 0) {
                    *out = e[i];
                    return 1;
                }
            }
        }

        if (cluster == 0) break;
        cluster = get_next_cluster(cluster);
    }

    return 0;
}

/* copy the whole file into dst (up to cap bytes); returns bytes written */
static uint32_t fat_read(const DirEntry *file, uint8_t *dst, uint32_t cap)
{
    uint16_t sect[256];
    uint32_t lba, count;
    uint16_t cluster   = file->low_cluster;
    uint32_t remaining = file->size;
    uint32_t written   = 0;

    while (remaining > 0 && cluster >= 0x0002 && cluster < 0xFFF8) {

        dir_location(cluster, &lba, &count);

        for (uint32_t s = 0; s < count && remaining > 0; s++) {

            ata_read_sector(lba + s, sect);

            uint32_t n = (remaining < bytes_per_sector) ? remaining : bytes_per_sector;
            if (written + n > cap) n = cap - written;

            uint8_t *src = (uint8_t *)sect;
            for (uint32_t i = 0; i < n; i++)
                dst[written + i] = src[i];

            written   += n;
            remaining -= n;

            if (written >= cap) return written;
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

void elf_inspect(const char *name, uint16_t dir_cluster)
{
    DirEntry de;

    if (!fat_find(name, dir_cluster, &de)) {
        sprint("elf: file not found\n", -1, -1);
        return;
    }
    if (de.size < sizeof(Elf32_Ehdr) || de.size > ELF_MAX_SIZE) {
        sprint("elf: bad file size\n", -1, -1);
        return;
    }

    uint8_t *image = (uint8_t *)(uintptr_t)fralloc(de.size);
    if ((uintptr_t)image == (uintptr_t)-1) {
        sprint("elf: no memory for image\n", -1, -1);
        return;
    }
    if (fat_read(&de, image, de.size) != de.size) {
        sprint("elf: short read\n", -1, -1);
        free((uint64_t *)(uintptr_t)image, de.size);
        return;
    }

    const Elf32_Ehdr *eh = (const Elf32_Ehdr *)image;

    line("file size ", de.size);

    sprint("magic     ", -1, -1);
    hprint(eh->e_ident[0]); hprint(eh->e_ident[1]);
    hprint(eh->e_ident[2]); hprint(eh->e_ident[3]);
    sprint("\n", -1, -1);

    if (eh->e_ident[0] != 0x7F || eh->e_ident[1] != 'E' ||
        eh->e_ident[2] != 'L'  || eh->e_ident[3] != 'F') {
        sprint("elf: not an ELF file\n", -1, -1);
        free((uint64_t *)(uintptr_t)image, de.size);
        return;
    }

    line("class     ", eh->e_ident[4]);   /* 1 = 32-bit        */
    line("data       ", eh->e_ident[5]);  /* 1 = little-endian */
    line("type      ", eh->e_type);       /* 2 = ET_EXEC       */
    line("machine   ", eh->e_machine);    /* 3 = EM_386        */
    line("entry     ", eh->e_entry);
    line("phoff     ", eh->e_phoff);
    line("phentsize ", eh->e_phentsize);
    line("phnum     ", eh->e_phnum);

    if (eh->e_phentsize < sizeof(Elf32_Phdr) ||
        eh->e_phoff + (uint32_t)eh->e_phnum * eh->e_phentsize > de.size) {
        sprint("elf: program header table out of range\n", -1, -1);
        free((uint64_t *)(uintptr_t)image, de.size);
        return;
    }

    for (int i = 0; i < eh->e_phnum; i++) {

        const Elf32_Phdr *ph =
            (const Elf32_Phdr *)(image + eh->e_phoff + (uint32_t)i * eh->e_phentsize);

        sprint("  seg ", -1, -1); hprint(i);
        sprint(" type ",   -1, -1); hprint(ph->p_type);      /* 1 = PT_LOAD */
        sprint(" off ",    -1, -1); hprint(ph->p_offset);
        sprint(" vaddr ",  -1, -1); hprint(ph->p_vaddr);
        sprint(" filesz ", -1, -1); hprint(ph->p_filesz);
        sprint(" memsz ",  -1, -1); hprint(ph->p_memsz);
        sprint(" flags ",  -1, -1); hprint(ph->p_flags);     /* 4=R 2=W 1=X */
        sprint("\n", -1, -1);
    }

    free((uint64_t *)(uintptr_t)image, de.size);
}
