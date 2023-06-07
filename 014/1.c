#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <signal.h>
#include <unistd.h>


int gl_cnt = 0;


void
handler(int s)
{
    if (gl_cnt == 5) {
        exit(0);
    }
    printf("%d\n", gl_cnt); fflush(stdout);
    ++gl_cnt;
}


int
main(void )
{
    sigaction(SIGHUP, 
            &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART},
            NULL);


    unsigned int wait_cnt = 0;
    pid_t mypid = getpid();
    printf("%lld\n", (long long)mypid); fflush(stdout);

    while (1) {
        ++wait_cnt;
        pause();
    }
    return 0;
}
