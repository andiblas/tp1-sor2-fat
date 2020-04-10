#include <stdio.h>
#include <stdlib.h>

const char *getPartitionType(int value)
{
    switch (value)
    {
    case 0x00:
        return "Empty";
    case 0x01:
        return "FAT12, CHS";
    case 0x04:
        return "FAT16, 16-32 MB, CHS";
    case 0x05:
        return "Microsoft Extended, CHS";
    case 0x06:
        return "FAT16, 32 MB-2 GB, CHS";
    case 0x07:
        return "NTFS";
    case 0x0b:
        return "FAT32, CHS";
    case 0x0c:
        return "FAT32, LBA";
    case 0x0e:
        return "FAT16, 32 MB-2 GB, LBA";
    case 0x0f:
        return "Microsoft Extended, LBA";
    case 0x11:
        return "Hidden FAT12, CHS";
    case 0x14:
        return "Hidden FAT16, 16-32 MB, CHS";
    case 0x16:
        return "Hidden FAT16, 32 MB-2 GB, CHS";
    case 0x1b:
        return "Hidden FAT32, CHS";
    case 0x1c:
        return "Hidden FAT32, LBA";
    case 0x1e:
        return "Hidden FAT16, 32 MB-2 GB, LBA";
    case 0x42:
        return "Microsoft MBR. Dynamic Disk";
    case 0x82:
        return "Linux Swap";
    case 0x83:
        return "Linux";
    case 0x84:
        return "Hibernation";
    case 0x85:
        return "Linux Extended";
    case 0x86:
        return "NTFS Volume Set";
    case 0x87:
        return "NTFS Volume Set";
    case 0xa0:
        return "Hibernation";
    case 0xa1:
        return "Hibernation";
    case 0xa5:
        return "FreeBSD";
    case 0xa6:
        return "OpenBSD";
    case 0xa8:
        return "Mac OSX";
    case 0xa9:
        return "NetBSD";
    case 0xab:
        return "Mac OSX Boot";
    case 0xb7:
        return "BSDI";
    case 0xb8:
        return "BSDI swap";
    case 0xee:
        return "EFI GPT Disk";
    case 0xef:
        return "EFI System Partition";
    case 0xfb:
        return "Vmware File System";
    case 0xfc:
        return "Vmware swap";
    default:
        return "";
    }
}

int main()
{
    FILE *in = fopen("test.img", "rb");
    unsigned int i, start_sector, length_sectors;

    // A partir del byte 446
    fseek(in, 446, SEEK_SET);

    int firstByte = fgetc(in);
    printf("Partition entry : First byte %02X (%s)\n", firstByte, firstByte == 128 ? "Booteable" : "No booteable");
    printf("  Comienzo de partición en CHS: %02X:%02X:%02X\n", fgetc(in), fgetc(in), fgetc(in));

    int partType = fgetc(in);
    printf("  Partition type 0x%02X (%s)\n", partType, getPartitionType(partType));
    printf("  Fin de partición en CHS: %02X:%02X:%02X\n", fgetc(in), fgetc(in), fgetc(in));

    fread(&start_sector, 4, 1, in);
    fread(&length_sectors, 4, 1, in);
    printf("  Dirección LBA relativa 0x%08X, de tamaño en sectores %d\n", start_sector, length_sectors);

    fclose(in);
    return 0;
}
