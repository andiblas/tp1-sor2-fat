.PHONY: all clean

SRC = read_boot.c read_mbr.c read_root.c read_first_partition.c read_files.c
BIN = $(SRC:.c=)

clean:
	rm -f $(BIN) $(OBJ)

all: 
	gcc read_boot.c -o read_boot
	gcc read_mbr.c -o  read_mbr
	gcc read_root.c -o read_root
	gcc read_first_partition.c -o read_first_partition
	gcc read_files.c -o read_files