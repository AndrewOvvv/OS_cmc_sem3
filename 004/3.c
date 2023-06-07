#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>


int
main(int argc, char *argv[])
{
    if (argc < 2) {
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY, 0);
    if (fd < 0) {
        exit(1);
    }
    
    
    return 0;
}
