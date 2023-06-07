#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char *argv[])
{
    if (argc < 2) {
        exit(1);
    }

    int fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0600);
    if (fd < 0) {
        exit(1);
    }

    unsigned int value;
    while (scanf("%u", &value) == 1) {
        unsigned char buf[] = 
        {
            value >> 20,
            value >> 12,
            ((value >> 8) & 0xf),
            value
        };
        if (write(fd, buf, sizeof(buf)) < 0) {
            exit(1);
        }
        
    }
    if (close(fd) < 0) {
        exit(1);
    }
    return 0;
}
