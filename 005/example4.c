#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

int
main(int argc, char *argv[]) 
{
    int fd = open(argv[1], O_RDONLY, 0);
    if (fd < 0) {
        exit(1);
    }
    
    struct stat stb;
    fstat(fd, &stb);
    if (!S_ISREG(stb.st_mode)) {
        exit(1);
    }
    if (stb.st_size == 0) {
        exit(0);
    }
    
    unsigned char *ptr = mmap(NULL, stb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        exit(1);
    }
    close(fd);

    int count = 0;
    for (int i = 0; i < stb.st_size; ++i) {
        if (ptr[i] == 'a') {
            ++count;
        }
    }
    munmap(ptr, stb.st_size);
    printf("%d\n", count);
    
    return 0;
}
