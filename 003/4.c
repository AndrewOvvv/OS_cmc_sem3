#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR, 0);
    if (fd < 0) {
        exit(-1);
    }

    long long size_of_file = (long long)lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    long long N = size_of_file / sizeof(N);
    if (N == 0) {
        return 0;
    }

    
    long long current_value, min_value, min_index = 0;
    ssize_t rd = read(fd, &current_value, sizeof(current_value));
    if (rd < 0) {
        exit(1);
    }
    min_value = current_value;

    for (int i = 1; i < N; ++i) {
        rd = read(fd, &current_value, sizeof(current_value));
        if (rd < 0) {
            exit(1);
        }

        if (current_value < min_value) {
            min_value = current_value;
            min_index = i;
        }
    }
    
    if (min_value != LLONG_MIN) { 
        min_value = -min_value;
    }
    lseek(fd, min_index * (long long)sizeof(current_value), SEEK_SET);
    if (write(fd, &min_value, sizeof(min_value)) != sizeof(min_value)) {
        exit(1);
    }

    if(close(fd) < 0) {
        exit(1);
    }
    return 0;
}
