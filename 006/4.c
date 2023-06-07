#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

enum
{
    BYTE_SIZE = 8
};


int
abs(int x) 
{
    if (x < 0) {
        return -x;
    } else {
        return x;
    }
}    


int
main(int argc, char *argv[]) 
{
    if (argc < 2) {
        exit(1);
    }
    
    int fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        exit(1);
    } 
    
    long long size_of_file = (long long)lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    long long bits_in_file = BYTE_SIZE * size_of_file;
    if (bits_in_file / BYTE_SIZE != size_of_file) {
        bits_in_file = LLONG_MAX;
    }
 
    int buf = 0;
    while (scanf("%d", &buf) != EOF) {
        if (abs(buf) <= bits_in_file) {
            int pos = ((abs(buf) - 1) / BYTE_SIZE);
            lseek(fd, pos, SEEK_SET);

            unsigned char cur_byte;
            if (read(fd, &cur_byte, sizeof(cur_byte)) < 0) {
                exit(1);
            }
            
            if (buf > 0) {
                cur_byte = (cur_byte | (1 << (abs(buf) - 1 - pos * BYTE_SIZE)));
            } else if (buf < 0) {
                cur_byte = (cur_byte | (1 << (abs(buf) - 1 - pos * BYTE_SIZE))); 
                cur_byte = (cur_byte ^ (1 << (abs(buf) - 1 - pos * BYTE_SIZE)));
            }
            
            lseek(fd, pos, SEEK_SET);
            if (write(fd, &cur_byte, sizeof(cur_byte)) != sizeof(cur_byte)) {
                exit(1);
            }
        }
    }
    if (close(fd) < 0) {
        exit(1);
    }
    return 0;
}
