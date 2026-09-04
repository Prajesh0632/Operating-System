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
uint32_t sectors_per_fat;
uint32_t total_clusters;


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
    sectors_per_fat = bpb->sectors_per_fat;
    data_start = root_start + root_sectors;


uint32_t total_sectors = bpb->total_sectors_short
                       ? bpb->total_sectors_short      // 16-bit field
                       : bpb->total_sectors_large;     // 32-bit, used when short is 0

total_clusters = (total_sectors - data_start) / sectors_per_cluster;
    

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

void set_filename(char* fname, char* name) {


    for(int i = 0; i < 11; i++) fname[i] = ' ';

    int len = 0;
    for(int i = 0; name[i] != '\0'; i++)len++;

    
    int f_index = 0;
    bool dot_found = false;
    int dot_index = -1;


    for(int i = 0; name[i] != '.' && name[i] != '\0' && i < 8; i++) {
        char c = name[i];
        if(c >= 'a' && c <= 'z') c -= 32;
        fname[f_index++] = c;

        if(name[f_index] == '.') {
            dot_found = true;
            dot_index = f_index;

        }
    }


    if(dot_found) {
            
        int index = 10;
        for(int i = len-1; name[i] != '.'; i-- ) {
            
            char c = name[i];
            if(c >= 'a' && c <= 'z') c -= 32;
            fname[index--] = c;
        }

    }

   
    
   

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


uint16_t get_free_cluster() {

    

    uint16_t buffer[256];
    
    for(uint32_t s = 0; s < sectors_per_fat; s++) {
        ata_read_sector(fat_start + s, buffer);

        for(uint32_t i = 0; i < 256; i++) {

            uint32_t c = s * 256 + i;

            if(c < 2) continue; // slots 0 and 1 aren't clusters

            if(c > total_clusters + 1) return 0; // past the end of the disk

            if(buffer[i] == 0x0000) {
                buffer[i] = 0xFFFF;
                ata_write_sector(fat_start + s, buffer);
                ata_write_sector(fat_start + s + sectors_per_fat, buffer);
                return c;
            }
        }

    }

    return 0; // disk full 
    

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


void create_file(char* filename, uint16_t cluster) {


  
    
     char fname[11];
     set_filename(fname, filename);



    uint16_t buffer[256];
    uint32_t lba, sector_count;
    uint32_t free_lba = 0, free_index = 0;
    bool found = false;

    while(cluster == 0 || (cluster >= 0x0002 && cluster < 0xFFF8)) {

        dir_location(cluster, &lba, &sector_count);

        for(uint32_t s = 0; s < sector_count && !found; s++) {

            ata_read_sector(lba + s, buffer);

            DirEntry* e = (DirEntry*)buffer;
            for(uint32_t i = 0; i < dir_per_sector; i++) {

                bool same = true;
        for (int k = 0; k < 11; k++)
            if (e[i].name[k] != (uint8_t)fname[k]) { same = false; break; }

        if (same) { sprint("Filename already exists\n", -1, -1); return; }

                if(e[i].name[0] == 0x00 || e[i].name[0] == 0xE5) {
                    free_lba = lba + s;
                    free_index = i;
                    found = true;
                    break;
                }
            }

            

            

        }


        if(!found) {
            cluster = get_next_cluster(cluster);
            continue;
        }

    

     ata_read_sector(free_lba, buffer);
     DirEntry* e = (DirEntry*)buffer;

     DirEntry* file = &e[free_index];
    
     for(uint32_t i = 0; i < 11; i++) {
         file->name[i] = (uint8_t)fname[i];
     }
     file->attribute = ATTR_ARCHIVE;
     file->size = 0;
     file->low_cluster = 0;




     ata_write_sector(free_lba, buffer);

     
     break;
     

    }
       

    if(!found) {
        sprint("Directory is full. Free some space before creating a file\n", -1, -1);
        
    }
        
   
    


     


}



void delete_file(char* filename, uint16_t cluster) {

    uint16_t buffer[256];

    uint32_t lba, sector_number;
    bool found = false;
    uint32_t file_lba, file_index;


    char fname[11];
    set_filename(fname, filename);


    //get file 
    while(cluster == 0 || (cluster >= 0x0002 && cluster < 0xFFF8)) {

        dir_location(cluster, &lba, &sector_number);

        for(uint32_t s = 0; s < sector_number && !found; s++) {

            ata_read_sector(lba + s, buffer);

            DirEntry* e = (DirEntry*)buffer;

            for(uint32_t i = 0; i < dir_per_sector; i++) {
            
            if(e[i].attribute == ATTR_DIRECTORY) continue;

            bool same = true;
            for (int k = 0; k < 11; k++)
                if (e[i].name[k] != (uint8_t)fname[k]) { same = false; break; }

            if(same) {
                found = true;
                file_lba = lba + s;
                file_index = i;
                break;
            }    

     }


     


                    


            }


            if(found)break;

            cluster = get_next_cluster(cluster);

        }



        if(!found) {
            sprint("File not found\n", -1, -1);
            return;
        }

       

        //delete file 

        ata_read_sector(file_lba, buffer);

        DirEntry* e = (DirEntry*)buffer;

        DirEntry* file = &e[file_index];


        cluster = file->low_cluster;

        while(cluster >= 0x0002 && cluster < 0xFFF8) {
          
          uint32_t next_cluster = get_next_cluster(cluster);

          uint16_t temp_buffer[256];


          uint32_t offset = cluster * 2;
          uint32_t lba = fat_start + offset / bytes_per_sector;

          ata_read_sector(lba, temp_buffer);


          temp_buffer[(offset % bytes_per_sector / 2)] = 0x0000;

          
          ata_write_sector(lba, temp_buffer);
          ata_write_sector(lba + sectors_per_fat, temp_buffer);



          cluster = next_cluster;
           
        }

        file->name[0] = 0xE5;

        ata_write_sector(file_lba, buffer);










    

    }



typedef struct{

    bool found;
    uint32_t lba, index;

}DirInfo;
    

DirInfo next_dir(char* dir, uint16_t cluster) {

    uint16_t buffer[256];
    uint32_t lba, sector_count;

   DirInfo direc;
   direc.found = false;

    while(cluster == 0 || (cluster >= 0x0002 && cluster < 0xFFF8)) {
           
           dir_location(cluster, &lba, &sector_count);

           for(uint32_t s = 0; s < sector_count; s++) {
               
            ata_read_sector(lba + s, buffer);

            DirEntry* e = (DirEntry*)buffer;

            for(uint32_t i = 0; i < dir_per_sector; i++) {

                if(!(e[i].attribute & ATTR_DIRECTORY)) continue;
                if(e[i].name[0] == 0x00 || e[i].name[0] == 0xE5) continue;

                char name[13];   // get_filename emits up to 8 + '.' + 3 + '\0'
                get_filename(name, e[i].name);

                if(strcmp(name, dir) == 0) {

                    direc.found = true;
                    direc.lba = lba + s;
                    direc.index = i;
                    return direc;

                }
            }

           }

           cluster = get_next_cluster(cluster);

    }


    return direc;

}


uint16_t change_dir(char* path, uint16_t cluster) {


 char path_buffer[32];
 int p_idx = 0;

 uint16_t original_cluster = cluster;

 DirInfo dir_info;


 for(int i = 0; path[i] != '\0'; i++) {

    if(path[i] != '\\') {

        path_buffer[p_idx++] = path[i];
    }  

    if(path[i] == '\\' || path[i+1] == '\0') {

    path_buffer[p_idx] = '\0';

    dir_info = next_dir(path_buffer, cluster);
    if(!dir_info.found) {
        sprint("No such Directory\n", -1, -1);
        return original_cluster;
    }

    uint16_t buffer[256];
    ata_read_sector(dir_info.lba, buffer);

    DirEntry* e = (DirEntry*)buffer;

    DirEntry* new_dir = &e[dir_info.index];

    cluster = new_dir->low_cluster;

    p_idx = 0;


    }
    


    


 }



 




  return cluster;
}
