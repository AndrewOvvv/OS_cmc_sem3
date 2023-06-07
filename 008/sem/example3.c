#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


int
main(void )
{
    int pid = fork();
    if (pid < 0) {
        abort();
    } 
    if (!pid) {
        int fd = open("/tmp/out.txt", O_WRONLY | O_TRUNC | O_CREAT, 0600);
        if (fd < 0) {
            _exit(1);
        }
        if (dup2(fd, STDOUT_FILENO) < 0) {
            _exit(1);
        }
        close(fd);
        if (chdir("/usr/bin") < 0) {
            _exit(1);
        }
        execlp("ls", "ls", "-l", NULL);
    }
    wait(NULL);
    return 0;
}
