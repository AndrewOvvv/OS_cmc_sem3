#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int
main(void )
{
    pid_t son = fork();
    if (!son) {
        pid_t gson = fork();
        if (!gson) {
            printf("3 "); fflush(stdout);
            _exit(0);
        }
        waitpid(gson, NULL, 0);
        printf("2 "); fflush(stdout);
        _exit(0);
    }
    waitpid(son, NULL, 0);
    printf("1\n"); fflush(stdout);
    return 0;
}
