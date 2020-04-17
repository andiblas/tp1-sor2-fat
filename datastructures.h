#define FILE_TO_OPEN "/dev/loop0"
//#define FILE_TO_OPEN "/dev/loop25" // Depende la maquina cambia el loop. 

typedef struct
{
    unsigned char first_byte;
    unsigned char start_chs[3];
    unsigned char partition_type;
    unsigned char end_chs[3];
    unsigned short starting_cluster;
    unsigned int file_size;
} __attribute((packed)) PartitionTable;

typedef struct
{
    char jmp[3];
    char oem[8];
    unsigned short sector_size;
    unsigned char sector_cluster;
    unsigned short reserved_sectors;
    unsigned char number_of_fats;
    unsigned short root_dir_files;
    unsigned short sectors_quantity_low;
    char medita_type;
    unsigned short fat_sector_size;
    unsigned short sectors_per_track;
    unsigned short heads;
    unsigned int sector_partition;
    unsigned int sectors_quantity_high;
    char bios_number;
    char not_used;
    char extended_boot_signature;
    unsigned int volume_id;
    char volume_label[11];
    char fs_type[8];
    char boot_code[448];
    unsigned short boot_sector_signature;
} __attribute((packed)) Fat12BootSector;

typedef struct
{
    unsigned char filename[8];
    unsigned char extension[3];
    unsigned char attributes[1];
    unsigned char reserved;
    unsigned char creation_time_seconds;
    unsigned char creation_time_hours_minutes_seconds[2];
    unsigned char creation_date[2];
    unsigned char access_date[2];
    unsigned short highorder_address;
    unsigned char modified_time[2];
    unsigned char modified_date[2];
    unsigned short loworder_address;
    unsigned int file_size;
} __attribute((packed)) Fat12Entry;
