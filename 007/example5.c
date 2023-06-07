#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int
main(void )
{
    int pid = fork();
    if (!pid) {
        _exit(0);
    }

    int status;
    waitpid(pid, &status, 0);
    /*
    if (!status) {
        // так писать плохо, так как нет документации строгой о скалировании данных в status
    }
    */

    if (WIFEXITED(status)) {  
        printf("exited with code: %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("terminated with signal: %d\n", WTERMSIG(status));
    }
    return 0;
}
