#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


enum 
{
    COUNT = 4,
    BLOCK_SIZE = 512
};


int fd_disks[4];
const char *disks[COUNT] = {"Disk0", "Disk1", "Disk2", "Disk3"};


void 
Ini_Raid0(void )
{
    for (int i = 0; i < COUNT; ++i) {
        fd_disks[i] = open(disks[i], O_RDWR);
    }
}


void 
Read_Raid0(int num, char *buf)
{
    unsigned file_number = num % COUNT;
    unsigned offset = BLOCK_SIZE * (num / COUNT);

    lseek(fd_disks[file_number], offset, SEEK_SET);
    read(fd_disks[file_number], buf, BLOCK_SIZE);
}
