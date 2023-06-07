#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/eventfd.h>


int
main(int argc, char *argv[])
{
    int count = strtoll(argv[1], NULL, 10);
    int *events = calloc(count + 1, sizeof(*events));
    int *sons_pid = calloc(count + 1, sizeof(*sons_pid));

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    for (int i = 0; i < count + 1; ++i) {
        events[i] = eventfd(0, 0);
    }

    for (int id = 0; id < count; ++id) {
        if (!(sons_pid[id] = fork())) {
            while (1) {
                uint64_t rvalue;
                int value;
                read(events[id], &rvalue, sizeof(rvalue));
                rvalue = 1;
                if (scanf("%d", &value) == EOF) {
                    write(events[count], &rvalue, sizeof(rvalue)); 
                } else {
                    printf("%d %d\n", id, value); 
                    int next_proc = (value % count + count) % count;
                    write(events[next_proc], &rvalue, sizeof(rvalue));
                }
            }
            _exit(0);
        }
    }

    uint64_t final = 1;
    write(events[0], &final, sizeof(final));
    read(events[count], &final, sizeof(final));

    for (int i = 0; i < count; ++i) {
        kill(sons_pid[i], SIGKILL);
        wait(NULL);
    }
    
    for (int i = 0; i <= count; ++i) {
        close(events[i]);
    }
    free(sons_pid);
    free(events);
    return 0;
}
