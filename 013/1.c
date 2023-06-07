#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>


enum 
{
    PREVS = 3,
    START_YEAR = 1900
};


int32_t
main(void )
{

    struct tm t = {};

    int32_t pipe_a[2];
    pipe(pipe_a);

    pid_t pid = fork();
    if (!pid) {
        // son
        pid_t gson = fork();
        if (!gson) {
            // gson
            pid_t ggson = fork();
            if (!ggson) {
                // ggson
                time_t tloc = time(NULL);
                for (int32_t prev = 0; prev < PREVS; ++prev) {
                    write(pipe_a[1], &tloc, sizeof(tloc));
                }
                _exit(0);
            } else {
                // gson
                waitpid(ggson, NULL, 0);
                time_t tloc;
                read(pipe_a[0], &tloc, sizeof(tloc));
                t = *localtime(&tloc);
                printf("D:%02d\n", t.tm_mday); fflush(stdout);
            }
            _exit(0);
        } else {
            //son
            waitpid(gson, NULL, 0);
            time_t tloc;
            read(pipe_a[0], &tloc, sizeof(tloc));
            t = *localtime(&tloc);
            printf("M:%02d\n", t.tm_mon + 1); fflush(stdout);
        }
        _exit(0);
    } else {
        // parent
        waitpid(pid, NULL, 0);
        time_t tloc;
        read(pipe_a[0], &tloc, sizeof(tloc));
        t = *localtime(&tloc);
        printf("Y:%04d\n", t.tm_year + START_YEAR); fflush(stdout);
    }
    return 0;
}
