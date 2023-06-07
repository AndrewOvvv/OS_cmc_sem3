#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>


int
main(void )
{
    printf("hello\n"); fflush(NULL);
    pid_t pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (!pid) {
        int fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600);
        if (fd < 0) {
            fprintf(stderr, "open failed\n");
            _exit(1);
        }
        // exec...
    }
    return 0;
}
