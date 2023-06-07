#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int
main(int argc, char *argv[])
{
    // ARGV[1] | ARGV[2]
    
    int fd[2];
    pipe2(fd, O_CLOEXEC);


    if (!fork()) {
        dup2(fd[0], 0);
        execlp(argv[2], argv[2], NULL);
        _exit(1);
    }
    if (!fork()) {
        dup2(fd[1], 1);
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }
    close(fd[0]);
    close(fd[1]);

    while (wait(NULL) > 0) {}
 
    return 0;
}
