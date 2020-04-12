#include <stdio.h>
#include <stdlib.h>
#include "datastructures.h"

int main() {
    FILE * in = fopen(FILE_TO_OPEN, "rb");
    int i;
    PartitionTable pt[4];
    Fat12BootSector bs;
    
    fseek(in, 446 , SEEK_SET); // Ir al inicio de la tabla de particiones. Completar ...
    fread(pt, sizeof(PartitionTable), 4, in); // leo entradas 
    
    for(i=0; i<4; i++) {        
        printf("Partition type: %d\n", pt[i].partition_type);
        if(pt[i].partition_type == 1) {
            printf("Encontrado FAT12 %d\n", i);
            break;
        }
    }
    
    if(i == 4) {
        printf("No se encontró filesystem FAT12, saliendo ...\n");
        return -1;
    }
    
    fseek(in, 0, SEEK_SET);
    fread(&bs, sizeof(Fat12BootSector), 1, in);
    
    printf("  Jump code: %02X:%02X:%02X\n", bs.jmp[0], bs.jmp[1], bs.jmp[2]);
    printf("  OEM code: [%.8s]\n", bs.oem);
    printf("  sector_size: %d\n", bs.sector_size);
    printf("  sector_cluster: %d\n", bs.sector_cluster);
    printf("  reserved_sectors: %d\n", bs.reserved_sectors);
    printf("  number_of_fats: %d\n", bs.number_of_fats);
    printf("  root_dir_files: %d\n", bs.root_dir_files);
    printf("  sectors_quantity_low: %d\n", bs.sectors_quantity_low);
    printf("  medita_type: %d\n", bs.medita_type);
    printf("  fat_sector_size: %d\n", bs.fat_sector_size);
    printf("  sectors_per_track: %d\n", bs.sectors_per_track);
    printf("  head: %d\n", bs.heads);
    printf("  sector_partition: %d\n", bs.sector_partition);
    printf("  sectors_quantity_high: %d\n", bs.sectors_quantity_high);
    printf("  bios_number: %d\n", bs.bios_number);
    printf("  not_used: %d\n", bs.not_used);
    printf("  extended_boot_signature: %d\n", bs.extended_boot_signature);
    printf("  volume_id: 0x%08X\n", (unsigned int)bs.volume_id);
    printf("  Volume label: [%.11s]\n", bs.volume_label);
    printf("  Filesystem type: [%.8s]\n", bs.fs_type);
    printf("  Boot sector signature: 0x%04X\n", bs.boot_sector_signature);
    
    fclose(in);
    return 0;
}
