#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

enum { BITS_IN_BYTE = 8 };

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


    lseek(fd, 0, SEEK_SET);
    off_t cur_byte = 0;
    off_t cur_seek = 1;
    uint8_t datum;
    if (read(fd, &datum, 1) != 1) {
        close(fd);
        exit(1);
    }


    for (uintmax_t i = 0; i < ncols * ncols; i += ncols + 1)
    {
        off_t byte = i / BITS_IN_BYTE;
        int bit = i % BITS_IN_BYTE;
        if (cur_byte != byte) {
            if (cur_byte != cur_seek) {
                cur_seek = cur_byte;
                lseek(fd, cur_byte, SEEK_SET);
            }
            if (write(fd, &datum, 1) != 1) {
                close(fd);
                exit(1);
            }
            cur_seek++;
            cur_byte = byte;
            if (cur_byte != cur_seek) {
                cur_seek = cur_byte;
                lseek(fd, cur_byte, SEEK_SET);
            }
            if (read(fd, &datum, 1) != 1) {
                close(fd);
                exit(1);
            }
            cur_seek++;
        }
        datum |= (1u << bit);
    }
    if (cur_byte != cur_seek) {
        cur_seek = cur_byte;
        lseek(fd, cur_byte, SEEK_SET);
    }
    if (write(fd, &datum, 1) != 1) {
        close(fd);
        exit(1);
    }

    close(fd);
    return 0;
}
