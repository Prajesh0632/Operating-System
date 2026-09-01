#include "fat16.h"
#include "ata.h"
#include "../screen_driver/screen.h"


uint32_t fat_start;
uint32_t root_start;
uint32_t root_sectors;
uint32_t data_start;
uint32_t dir_per_sector;
uint32_t sectors_per_cluster;


void init_fat16() {

    uint16_t buffer[256];

    ata_read_sector(0, buffer);

    Bpb* bpb = (Bpb*)buffer;

    fat_start = bpb->reserved_sectors;
    root_start = fat_start + (bpb->fat_n * bpb->sectors_per_fat);
    root_sectors = (bpb->directories_n * sizeof(DirEntry) + bpb->bytes_per_sector - 1) / (bpb->bytes_per_sector);
    dir_per_sector = bpb->bytes_per_sector / sizeof(DirEntry);
    sectors_per_cluster = bpb->sectors_per_cluster;
    data_start = root_start + root_sectors;


}

void dir_location(uint32_t cluster, uint32_t* lba, uint32_t* sector_count) {

    if(cluster == 0) {
        *lba = root_start;
        *sector_count = root_sectors;


    }

    else {
        *lba = data_start + (cluster - 2) * sectors_per_cluster;
        *sector_count = sectors_per_cluster;
    }
}



void list_dir(uint32_t cluster) {
     

        uint16_t buffer[256];
        uint32_t lba, sector_count;

        dir_location(cluster, &lba, &sector_count);


        for(uint32_t s = 0; s < sector_count; s++) {

        ata_read_sector(lba + s, buffer);

        DirEntry* e = (DirEntry*)buffer;

        for(uint32_t i = 0; i < dir_per_sector; i++) {
        if (e[i].name[0] == 0x00) return;                  // end of directory
        if (e[i].name[0] == 0xE5) continue;                // deleted
        if (e[i].attribute == ATTR_LFN) continue;           // long-name fragment
        if (e[i].attribute & ATTR_VOLUME_ID) continue;      // the label
        
        for(int j = 0; j < 11; j++) {

           if(j == 8 && e[i].name[j] != ' ') cprint('.'); 
           if(e[i].name[j] != ' ') cprint(e[i].name[j]);
           

        }

        cprint('\n');

        


        }

        }
            
       


}