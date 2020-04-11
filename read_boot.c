#include <stdio.h>
#include <stdlib.h>
#include "datastructures.h"

int main() {
    FILE * in = fopen("test.img", "rb");
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
    printf("  root_dir_entries: %d\n", bs.root_dir_entries);
    printf("  sector_volumen: %d\n", bs.sector_volumen);
    printf("  descriptor: %d\n", bs.descriptor);
    printf("  fat_size_sectors: %d\n", bs.fat_size_sectors);
    printf("  sector_track: %d\n", bs.sector_track);
    printf("  headers: %d\n", bs.headers);
    printf("  sector_hidden: %d\n", bs.sector_hidden);
    printf("  sector_partition: %d\n", bs.sector_partition);
    printf("  physical_device: %d\n", bs.physical_device);
    printf("  current_header: %d\n", bs.current_header);
    printf("  firm: %d\n", bs.firm);
    printf("  volume_id: 0x%08X\n", (unsigned int)bs.volume_id);
    printf("  Volume label: [%.11s]\n", bs.volume_label);
    printf("  Filesystem type: [%.8s]\n", bs.fs_type);
    printf("  Boot sector signature: 0x%04X\n", bs.boot_sector_signature);
    
    fclose(in);
    return 0;
}
