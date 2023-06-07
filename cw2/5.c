#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/eventfd.h>


int
main(int argc, char *argv[])
{
    long long N = strtoll(argv[1], NULL, 10);
    long long L = strtoll(argv[2], NULL, 10);
    long long alive = N;
    int *events = calloc(N + 1, sizeof(*events));
    int *sons_pid = calloc(N + 1, sizeof(*sons_pid));

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    for (int i = 0; i < N + 1; ++i) {
        events[i] = eventfd(0, 0);
    }

    for (int i = 0; i < N; ++i) {
        if (!(sons_pid[i] = fork())) {
            while (1) {
                uint64_t rvalue;
                read(events[i], &rvalue, sizeof(rvalue));
                --L;
                if (L == 0) {
                    rvalue = 2;
                } else {
                    rvalue = 1;
                }
                printf("%d %lld\n", i, L);
                write(events[N], &rvalue, sizeof(rvalue)); 
                if (L == 0) {
                    _exit(0);
                }
            }
            _exit(0);
        }
    }

    unsigned int killed;
    while (scanf("%u", &killed) != EOF) {
        unsigned int number = killed % N;
        uint64_t final = 1;
        write(events[number], &final, sizeof(final));
        read(events[N], &final, sizeof(final));
        if (final != 1) {
            --alive;
            waitpid(sons_pid[number], NULL, 0);
            sons_pid[number] = -1;
        }
    }

    printf("%lld", alive);
    for (int i = 0; i < N; ++i) {
        if (sons_pid[i] > 0) {
            kill(sons_pid[i], SIGKILL);
            wait(NULL);
        }
    }
    
    for (int i = 0; i <= N; ++i) {
        close(events[i]);
    }
    free(sons_pid);
    free(events);
    return 0;
}
