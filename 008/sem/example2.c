#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


int
sys(const char *command)
{
    pid_t pid = fork();
    if (pid < 0) {
        return -1;
    }
    if (!pid) {
        execl("/bin/sh/", "sh", "-c", command, NULL);
        _exit(1);
    }
    int status = 0;
    int r = waitpid(pid, &status, 0);
    if (r > 0) {
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            return 128 + WTERMSIG(status);
        } else {
            abort();
        }
    } else {
        abort();
    }
}


int
main(void )
{
    sys("cat -");
    return 0;
}
