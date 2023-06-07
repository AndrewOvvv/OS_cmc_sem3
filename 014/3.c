
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


long long
min(long long x, long long y)
{
    if (x < y) {
        return x;
    }
    return y;
}


int
main(int argc, char *argv[])
{
    int status;
    long long cnt_success = 0;
    long long N = strtoll(argv[1], NULL, 10);

    for (int i = 2; i < min(N + 2, argc); ++i) {
        FILE* fd = fopen(argv[i], "r");
        if (!fd) {
            continue;
        }
        char path[PATH_MAX] = {0};
        
        fscanf(fd, "%s", path);
        fclose(fd);

        if (!fork()) {
            execlp(path, path, NULL);
            _exit(1);
        }
    }

    while (wait(&status) > 0) {
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            ++cnt_success;
        }
    }

    for (int i = N + 2; i < argc; ++i) {
        FILE* fd = fopen(argv[i], "r");
        if (!fd) {
            continue;
        }
        char path[PATH_MAX] = {0};
        
        fscanf(fd, "%s", path);
        fclose(fd);

        if (!fork()) {
            execlp(path, path, NULL);
            _exit(1);
        }

        //...
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            ++cnt_success;
        }
    }
    printf("%lld\n", cnt_success); fflush(stdout);
    return 0;
}
