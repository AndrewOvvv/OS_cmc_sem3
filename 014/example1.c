#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/eventfd.h>


int
main(int argc, char *argv[])
{
    int n = strtol(argv[1], NULL, 10);
    int *fds = calloc(n, sizeof(fds[0]));
    _Atomic char *stat = mmap(NULL, n + 1, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    memset(stat, '.', n);
    stat[n] = 0;

    unsigned long long v1, s0, s1;

    for (int i = 0; i < n; ++i) {
        fds[i] = eventfd(1, 0);
    }
    
    for (int i = 0; i < n; ++i) {
        if (!fork()) {
            while (1) {
                stat[i] = 'W'; //wait status

                // try to read and choose forks (and order of forks
                s0 = i;
                s1 = (i + 1) % n;
                if (s0 > s1) {
                    s0 = (i + 1) % n;
                    s1 = i;
                }


                read(fds[s0], &v1, sizeof(v1));
                read(fds[s1], &v1, sizeof(v1));
                // if try was successful -> start eating
                stat[i] = 'E';
                sched_yield();
                stat[i] = 'S';
                v1 = 1;
                // try to write new status
                write(fds[s1], &v1, sizeof(v1));
                write(fds[s0], &v1, sizeof(v1));



                //...
                sched_yield();
            }
            _exit(0);
        }
    }

    while (1) {
        printf("%s\r", stat);
        sched_yield();
    }
    return 0;
}

