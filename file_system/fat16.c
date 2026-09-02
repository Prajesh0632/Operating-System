#include "fat16.h"
#include "ata.h"
#include "../screen_driver/screen.h"
#include <stdbool.h>
#include "../headers/string/str.h"


uint32_t fat_start;
uint32_t root_start;
uint32_t root_sectors;
uint32_t data_start;
uint32_t dir_per_sector;
uint32_t sectors_per_cluster;
uint32_t bytes_per_sector;


void init_fat16() {

    uint16_t buffer[256];

    ata_read_sector(0, buffer);

    Bpb* bpb = (Bpb*)buffer;


    bytes_per_sector = bpb->bytes_per_sector;
    fat_start = bpb->reserved_sectors;
    root_start = fat_start + (bpb->fat_n * bpb->sectors_per_fat);
    root_sectors = (bpb->directories_n * sizeof(DirEntry) + bpb->bytes_per_sector - 1) / (bpb->bytes_per_sector);
    dir_per_sector = bpb->bytes_per_sector / sizeof(DirEntry);
    sectors_per_cluster = bpb->sectors_per_cluster;
    data_start = root_start + root_sectors;


}


void get_filename(char* fname, char* name) {
    
    int index = 0;

    for(uint32_t i = 0; i < 11; i++) {

        if(i == 8 && name[i] != ' ') {
            fname[index++] = '.';
        }

        if(name[i] != ' ') {
            fname[index++] = name[i];
        }
    }

    fname[index] = '\0';
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


uint16_t get_next_cluster(uint16_t cluster) {

 uint16_t buffer[256];
 uint32_t offset = cluster * 2;

 ata_read_sector(fat_start + (offset / bytes_per_sector), buffer);

 return buffer[(offset % bytes_per_sector) / 2];



}



void list_dir(uint32_t cluster) {

    uint16_t buffer[256];
    uint32_t lba, sector_count;
        



        
       while(cluster == 0 || (cluster >= 0x0002 && cluster < 0xFFF8)) {

        dir_location(cluster, &lba, &sector_count);

        bool end_of_dir = false;


        for(uint32_t s = 0; s < sector_count && !end_of_dir; s++) {

        ata_read_sector(lba + s, buffer);

        DirEntry* e = (DirEntry*)buffer;

        for(uint32_t i = 0; i < dir_per_sector; i++) {
        if (e[i].name[0] == 0x00) {   // end of directory
             
            end_of_dir = true;
            continue;

        };               
        if (e[i].name[0] == 0xE5) continue;                // deleted
        if (e[i].attribute == ATTR_LFN) continue;           // long-name fragment
        if (e[i].attribute & ATTR_VOLUME_ID) continue;      // the label

        char name[12];
        get_filename(name, e[i].name);

        sprint(name, -1, -1);
        
        cprint('\n');

        


        }

        }

        if(end_of_dir || cluster == 0)  break;

        cluster = get_next_cluster(cluster);
        


       }
     

        

        
            
       


}

void display_file(DirEntry* file) {

    uint16_t buffer[256];
    uint32_t lba, sector_count;

    uint16_t cluster = file->low_cluster;
    uint32_t remaining = file->size;


    while(remaining > 0 && cluster >= 0x0002 && cluster < 0xFFF8) {
       
        dir_location(cluster, &lba, &sector_count);


        for(uint32_t s = 0; s < sector_count && remaining > 0; s++) {
              
            ata_read_sector(lba + s, buffer);
            uint8_t* content = (uint8_t*)buffer;

            uint32_t size = (remaining < bytes_per_sector) ? remaining : bytes_per_sector;
            for(uint32_t i = 0; i < size; i++) {
                cprint(content[i]);
            }

            remaining -= size;

        }

        cluster = get_next_cluster(cluster);




    }




}



void print_file(char* filename, uint16_t cluster) {


    uint16_t buffer[256];

    uint32_t lba, sector_count;
    DirEntry file;

     bool file_found = false;


     while(cluster == 0 || (cluster >= 0x0002 && cluster < 0xFFF8)) {


        

        dir_location(cluster, &lba, &sector_count);
        bool end_of_dir = false;
       

        
        for(uint32_t s = 0; s < sector_count; s++) {

             ata_read_sector(lba + s, buffer);
                DirEntry* e = (DirEntry*)buffer; 

        for(uint32_t i = 0; i < dir_per_sector; i++) {
            if (e[i].name[0] == 0x00) {   // end of directory
                
                end_of_dir = true;
                continue;

            };     

            if (e[i].name[0] == 0xE5) continue;                // deleted
            if (e[i].attribute == ATTR_LFN) continue;           // long-name fragment
            if (e[i].attribute & ATTR_VOLUME_ID) continue;      // the label
            
            char name[12];
            get_filename(name, e[i].name);

            
            if(strcmp(name, filename) == 0) {
                  
                file_found = true;
                file = e[i];
                break;


            }



            
        }



    


     

    }
        if (file_found) break;
        if (end_of_dir) break;

        if (cluster == 0) break;
        cluster = get_next_cluster(cluster);

    }


    if(file_found) {
       display_file(&file);
    }
    else {
        sprint("No such file found\n", -1, -1);
    }



}
