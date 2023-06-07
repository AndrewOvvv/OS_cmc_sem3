#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR, 0);
    if (fd < 0) {
        exit(-1);
    }

    char *eptr = NULL;
    errno = 0;
    long N = strtol(argv[2], &eptr, 10);
    if (errno || *eptr || eptr == argv[2] || (int) N != N) {
        exit(1);
    }

    //checking count of numbers
    long long size_of_file = (long long)lseek(fd, 0, SEEK_END);
    if (N * sizeof(double) > size_of_file) {
        N = size_of_file / sizeof(double);
    }
    lseek(fd, 0, SEEK_SET);
    //end of checking
    
    unsigned int start_read = 0;
    double previous_write_value = 0;
    double current_read_value = 0;
    for (int i = 0; i < N; ++i) {
        ssize_t r = read(fd, &current_read_value, sizeof(current_read_value));
        if (r < 0) { 
            exit(1);
        }
        previous_write_value = current_read_value - previous_write_value;
        lseek(fd, start_read, SEEK_SET);
        if (write(fd, &previous_write_value, sizeof(previous_write_value)) != sizeof(previous_write_value)) {
            exit(1); 
        }
        start_read += sizeof(current_read_value);
    }
    if(close(fd) < 0) {
        exit(1);
    }
    return 0;
}
