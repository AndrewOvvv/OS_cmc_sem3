#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


void 
start_son(char *name, int *status)
{
    pid_t son = fork();
    if (son < 0) {
        exit(1);
    } else if (!son) {
        execlp(name, name, NULL);
        _exit(1);
    }
    if (wait(status) < 0) {
        exit(1);
    }
}

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        exit(1);
    }
    
    int status;

    pid_t son_cmd1 = fork();
    if (son_cmd1 < 0) {
        exit(1);
    } else if (!son_cmd1) {
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }
    if (wait(&status) < 0) {
        exit(1);
    }
    
    if (!(WIFEXITED(status) && (WEXITSTATUS(status) == 0))) {
        // if previous id false
        start_son(argv[2], &status);
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        // if previous is true
        start_son(argv[3], &status);
    }

    //final check
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        exit(0);
    } else {
        exit(1);
    }
    return 0;
}
