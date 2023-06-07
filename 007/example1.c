#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int
main(void )
{
    pid_t pid = fork();
    if (pid < 0) {
        exit(1);
    }
    printf("%d, %d, %d\n", pid, getpid(), getppid());
    return 0;
}
