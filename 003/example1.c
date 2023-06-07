
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    // 0600 - only you
    // 0660 - you and your group
    // 0666 - rd and wr for all
    int fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0600); //last arg set access rights for file
    if (fd < 0) {
        fprintf(stderr, "open %s failed %s\n", argv[1], strerror(errno));
        exit(1);
    }
    write(fd, "hello\n", 6);
    close(fd);
}
