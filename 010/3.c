#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>


int32_t
arg_to_int(char *arg)
{
    char *eptr = NULL;
    errno = 0;
    long value = strtol(arg, &eptr, 10);
    if (errno || *eptr || eptr == arg || (int32_t) value != value) {
        exit(1);
    }
    return value;
}


int32_t
main(int32_t argc, char *argv[])
{
    if (argc < 4) {
        exit(1);
    }

    int32_t fd = open(argv[1], O_RDWR | O_TRUNC | O_CREAT, 0644);
    if (fd < 0) {
        exit(1);
    }
    int32_t rows = arg_to_int(argv[2]), columns = arg_to_int(argv[3]);
 
    char *buf = malloc(sizeof(int32_t) * rows * columns);
    write(fd, buf, sizeof(int32_t) * rows * columns);
    free(buf);

    int32_t *ptr = mmap(NULL, sizeof(*ptr) * rows * columns, PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        exit(1);
    }
    close(fd);

    int32_t cnt = 1, type = 1; // 1 - right, -1 - left, 2 - down, -2 - up 
    int32_t cur_r = 0, cur_c = 0;
    int32_t filled_rows_pref = 0, filled_rows_suff = 0;
    int32_t filled_cols_pref = 0, filled_cols_suff = 0;
    while (cnt < rows * columns) {
        if (type == 1) {
            for (; cur_c < columns - filled_cols_suff - 1; ++cur_c) {
                ptr[cur_r * columns + cur_c] = cnt;
                ++cnt;
            }
            ++filled_rows_pref;
            type = 2;
        } else if (type == 2) {
            for (; cur_r < rows - filled_rows_suff - 1; ++cur_r) {
                ptr[cur_r * columns + cur_c] = cnt;
                ++cnt; 
            }
            ++filled_cols_suff;
            type = -1;
        } else if (type == -1) {
            for (; filled_cols_pref < cur_c; --cur_c) {
                ptr[cur_r * columns + cur_c] = cnt;
                ++cnt;
            }
            ++filled_rows_suff;
            type = -2;
        } else {
            for (; filled_rows_pref < cur_r; --cur_r) {
                ptr[cur_r * columns + cur_c] = cnt;
                ++cnt;
            }
            ++filled_cols_pref;
            type = 1;
        }
    }
    ptr[cur_r * columns + cur_c] = cnt;
    munmap(ptr, sizeof(*ptr) * rows * columns);

    return 0;
}
