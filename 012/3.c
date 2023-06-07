#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>


int32_t
main(int32_t argc, char *argv[])
{
    int32_t cnt_proc = 0, cnt_success = 0, status;
    for (int32_t proc = 1; proc < argc; ++proc) {
        if (argv[proc][0] == 'p') {
            ++cnt_proc;
            pid_t pid = fork();
            if (pid < 0) {
                exit(1);
            } else if (!pid) {
                execlp(argv[proc] + 1, argv[proc] + 1, NULL);
                _exit(1);
            }
        } else {
            while (cnt_proc--) {
                wait(&status);
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    ++cnt_success;
                }
            }
            cnt_proc = 0;

            pid_t pid = fork();
            if (pid < 0) {

            } else if (!pid) {
                execlp(argv[proc] + 1, argv[proc] + 1, NULL);
                _exit(1);
            }
            waitpid(pid, &status, 0);

            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                ++cnt_success;
            }
        }
    }

    while (cnt_proc--) {
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            ++cnt_success;
        }
    }
    cnt_proc = 0;

    printf("%d\n", cnt_success);
    return 0;
}
