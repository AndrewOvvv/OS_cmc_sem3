#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


enum
{
    BITS_IN_BYTE = 8 
};


int
main(int argc, const char** argv)
{
    if (argc < 2) {
        exit(1);
    }


    int fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        exit(1);
    }
    off_t size = lseek(fd, 0, SEEK_END);
    if (size < 0) {
        close(fd);
        exit(1);
    }


    uintmax_t sz = size;
    if ((sz * BITS_IN_BYTE) / BITS_IN_BYTE != sz) {
        close(fd);
        exit(1);
    }
    sz *= BITS_IN_BYTE;
    if (sz == 0) {
        close(fd);
        return 0;
    }

    uintmax_t ncols = 1;
    while (ncols * ncols < sz) {
        ++ncols;
    }

    for (uintmax_t i = 0; i < ncols * ncols; i += ncols + 1)
    {
        off_t byte = i / BITS_IN_BYTE;
        int bit = i % BITS_IN_BYTE;
        uint8_t datum;
        lseek(fd, byte, SEEK_SET);
        if (read(fd, &datum, 1) != 1) {
            close(fd);
            exit(1);
        }
        datum |= (1u << bit);
        lseek(fd, byte, SEEK_SET);
        if (write(fd, &datum, 1) != 1) {
            close(fd);
            exit(1);
        }
    }

    close(fd);
    return 0;
}
