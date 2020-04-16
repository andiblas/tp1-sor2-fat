#include <stdio.h>
#include <stdlib.h>
#include "datastructures.h"

void print_file_content(Fat12Entry *entry, int data_area, int cluster_size)
{
    if (entry->highorder_address > 0 || entry->file_size <= 0)
    {
        return;
    }
    printf("file_size %d\n", entry->file_size);
    char content[entry->file_size];
    int content_dir = data_area + ((entry->loworder_address - CLUSTER_ADDRESS_START) * cluster_size);
    printf("content_dir [%d]\n", content_dir);
    FILE *file = fopen(FILE_NAME, "rb");
    fseek(file, content_dir, SEEK_SET);
    fread(content, entry->file_size, 1, file);
    printf("cluster [%d], content: [%s]\n", entry->first_cluster_lsb, content);
    fclose(file);
}

void print_files_only(Fat12Entry *entry) {
    switch(entry->filename[0]) {
        case 0x00:
        case 0x05:
        case 0xE5:
            return;
        default:
            switch(entry->attributes[0]) {
                case 0x20:
                    print_file_contents(entry);
                    return;
                default:
                    return;
            }
    }
}

int main() {
    FILE * in = fopen(FILE_TO_OPEN, "rb");
    int i;
    PartitionTable pt[4];
    Fat12BootSector bs;
    Fat12Entry entry;

    fseek(in, 446 , SEEK_SET); // Ir al inicio de la tabla de particiones. Completar ...
    fread(pt, sizeof(PartitionTable), 4, in); // leo entradas 
    
    for(i=0; i<4; i++) {        
        if(pt[i].partition_type == 1) {
            printf("Encontrada particion FAT12 %d\n", i);
            break;
        }
    }
    
    if(i == 4) {
        printf("No encontrado filesystem FAT12, saliendo...\n");
        return -1;
    }
    
    fseek(in, 0, SEEK_SET);
    fread(&bs, sizeof(Fat12BootSector), 1, in);
    
    printf("En  0x%X, sector size %d, FAT size %d sectors, %d FATs\n\n", 
           ftell(in), bs.sector_size, bs.fat_sector_size, bs.number_of_fats);
    
    fseek(in, (bs.reserved_sectors-1 + bs.fat_sector_size * bs.number_of_fats) *
          bs.sector_size, SEEK_CUR);
    
    printf("Root dir_entries %d \n", bs.root_dir_files);
    for(i=0; i<bs.root_dir_files; i++) {
        fread(&entry, sizeof(entry), 1, in);
        print_files_only(&entry);
    }
    
    printf("\nLeido Root directory, ahora en 0x%X\n", ftell(in));
    fclose(in);
    return 0;
}
