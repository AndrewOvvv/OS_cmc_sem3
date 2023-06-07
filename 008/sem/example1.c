#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>


int
main(int argc, char *argv[], char *envp[])
{
    //execve(argv[1], argv + 1. envp);
    execvp(argv[1], argv + 1); //использует для поиска глобальные пути
    fprintf(stderr, "execve failed: %s\n", strerror(errno));
    return 0;
}
