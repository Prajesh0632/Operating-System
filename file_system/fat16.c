#include "fat16.h"
#include "ata.h"
#include "../screen_driver/screen.h"


uint32_t fat_start;
uint32_t root_start;
uint32_t root_sectors;
uint32_t data_start;
uint32_t roots_per_sector;


void init_fat16() {

    uint16_t buffer[256];

    ata_read_sector(0, buffer);

    Bpb* bpb = (Bpb*)buffer;

    fat_start = bpb->reserved_sectors;
    root_start = fat_start + (bpb->fat_n * bpb->sectors_per_fat);
    root_sectors = (bpb->directories_n * 32 + bpb->bytes_per_sector - 1) / (bpb->bytes_per_sector);
    roots_per_sector = bpb->bytes_per_sector / sizeof(DirEntry);
    data_start = root_start + root_sectors;


}

void read_directory() {
     
    for(int s = 0; s < root_sectors; s++) {

        uint16_t buffer[256];

            
        ata_read_sector(root_start + s, buffer);

        DirEntry* e = (DirEntry*)buffer;

        for(int i = 0; i < roots_per_sector; i++) {
        if (e[i].name[0] == 0x00) return;                  // end of directory
        if (e[i].name[0] == 0xE5) continue;                // deleted
        if (e[i].attribute == ATTR_LFN) continue;           // long-name fragment
        if (e[i].attribute & ATTR_VOLUME_ID) continue;      // the label

        sprint((char*)(uintptr_t)e[i].name, -1, -1);



        }

    }

}