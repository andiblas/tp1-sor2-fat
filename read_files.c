#include <stdio.h>
#include <stdlib.h>
#include "datastructures.h"

void print_file_contents(Fat12Entry *entry, Fat12BootSector *bootSector)
{
    // comenzamos a calcular donde comienzan los datos.
    int dataArea = (bootSector->reserved_sectors + bootSector->fat_sector_size * bootSector->number_of_fats) * bootSector->sector_size;
    dataArea += bootSector->root_dir_files * sizeof(Fat12Entry);
    int clusterSize = bootSector->sector_size * bootSector->sector_cluster;

    // los clusters comienzan desde el dos, es por eso que sustraemos 2.
    int fileContentPhysicalAddress = dataArea + ((entry->loworder_address - 2) * clusterSize);
    FILE *file = fopen(FILE_TO_OPEN, "rb");
    fseek(file, fileContentPhysicalAddress, SEEK_SET);

    char content[entry->file_size];
    fread(content, entry->file_size, 1, file);
    printf("Filename of %s:\n", entry->filename);
    printf("File size of %d:\n", entry->file_size);
    printf("%s\n", content);
    fclose(file);
}

void print_files_only(Fat12Entry *entry, Fat12BootSector *bootSector)
{
    if (entry->attributes[0] == 0x20)
    {
        print_file_contents(entry, bootSector);
    }
}

void print_deleted_files(Fat12Entry *entry, Fat12BootSector *bootSector)
{
    if ((entry->filename[0] == 0xE5) ) {
        if (entry->attributes[0] == 0x20)
        {
            print_file_contents(entry, bootSector);
        }
    }
}

int main()
{
    FILE *in = fopen(FILE_TO_OPEN, "rb");
    int i;
    PartitionTable pt[4];
    Fat12BootSector bs;
    Fat12Entry entry;

    fseek(in, 446, SEEK_SET);                 // Ir al inicio de la tabla de particiones. Completar ...
    fread(pt, sizeof(PartitionTable), 4, in); // leo entradas

    for (i = 0; i < 4; i++)
    {
        if (pt[i].partition_type == 1)
        {
            printf("Encontrada particion FAT12 %d\n", i);
            break;
        }
    }

    if (i == 4)
    {
        printf("No encontrado filesystem FAT12, saliendo...\n");
        return -1;
    }

    fseek(in, 0, SEEK_SET);
    fread(&bs, sizeof(Fat12BootSector), 1, in);

    printf("En  0x%X, sector size %d, FAT size %d sectors, %d FATs\n\n",
           ftell(in), bs.sector_size, bs.fat_sector_size, bs.number_of_fats);

    fseek(in, (bs.reserved_sectors - 1 + bs.fat_sector_size * bs.number_of_fats) * bs.sector_size, SEEK_CUR);

    printf("Root dir_entries %d \n", bs.root_dir_files);
    for (i = 0; i < bs.root_dir_files; i++)
    {
        fread(&entry, sizeof(entry), 1, in);
        print_files_only(&entry, &bs);
    }
    
    fseek(in, 0, SEEK_SET);
    fread(&bs, sizeof(Fat12BootSector), 1, in);
    fseek(in, (bs.reserved_sectors - 1 + bs.fat_sector_size * bs.number_of_fats) * bs.sector_size, SEEK_CUR);
    
    printf("Deleted files: \n");
    for (i = 0; i < bs.root_dir_files; i++)
    {
        fread(&entry, sizeof(entry), 1, in);
        print_deleted_files(&entry, &bs);
    }


    printf("\nLeido Root directory, ahora en 0x%X\n", ftell(in));
    fclose(in);
    return 0;
}
