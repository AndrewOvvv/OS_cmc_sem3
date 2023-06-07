#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/eventfd.h>


enum { CNT_PRM = 2 };


int
pow_mod(int value, int cnt, int MOD)
{
    int result = 1;
    for (int i = 0; i < cnt; ++i) {
        result *= value;
        result %= MOD;
    }
    return (result + MOD) % MOD;
}


int
main(int argc, char *argv[])
{
    uint64_t buf_value = 1;

    int cnt_proc = strtoll(argv[1], NULL, 10);
    int max_value = strtoll(argv[3], NULL, 10);


    int *sons_pid = calloc(cnt_proc + 1, sizeof(*sons_pid));
    int *events = calloc(cnt_proc + 1, sizeof(*events));
    for (int i = 0; i <= cnt_proc; ++i) {
        events[i] = eventfd(0, 0);
    }
    int *ptr = mmap(NULL, sizeof(*ptr) * CNT_PRM, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);


    for (int id = 1; id <= cnt_proc; ++id) {
        if (!(sons_pid[id] = fork())) {
            while(1) {
                // read
                read(events[id], &buf_value, sizeof(buf_value));
                
                // start CRIT_SEC 

                int value = ptr[0], parent = ptr[1];
                printf("%d %d %d\n", id, value, parent);

                ++value;
                int next_proc = pow_mod(value, 4, cnt_proc) + 1;
                ptr[0] = value;
                ptr[1] = id;
                if (value > max_value) {
                    next_proc = 0;
                }
                // finish CRIT_SEC

                // write 
                write(events[next_proc], &buf_value, sizeof(buf_value));
            } 
        }
    }

    ptr[0] = 0;
    ptr[1] = 0;
    write(events[1], &buf_value, sizeof(buf_value));
    read(events[0], &buf_value, sizeof(buf_value));
    munmap(ptr, sizeof(*ptr) * CNT_PRM);


    for (int i = 1; i <= cnt_proc; ++i) {
        kill(sons_pid[i], SIGKILL);
        wait(NULL);
    }

    for (int i = 0; i <= cnt_proc; ++i) {
        close(events[i]);
    }

    free(sons_pid);
    free(events);
    return 0;
}
