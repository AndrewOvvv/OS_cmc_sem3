#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>


int fd[2];
int serial;
int pid;


void
handler(int s)
{
    int v;

    if (!pid) {
        pid = getppid();
    }
    
    read(fd[0], &v, sizeof(v));
    //write(STDOUT_FILENO, "!!!\n", 4);
    printf("%d: %d\n", serial, v); fflush(stdout);
    ++v;
    write(fd[1], &v, sizeof(v));
    kill(pid, SIGUSR1);
}


int
main(void )
{
    sigset_t ss1, ss2;

    sigemptyset(&ss1);
    sigemptyset(&ss2);
    sigaddset(&ss1, SIGUSR1);
    sigprocmask(SIG_BLOCK, &ss1, NULL);

    pipe(fd);
    sigaction(SIGUSR1, 
            &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART},
            NULL);
    if (!(pid = fork())) {
        // son
        serial = 2;
        pid = getppid();
        while (1) {
            sigsuspend(&ss2);
            }
    } else {
        // parent
        serial = 1;
        {
            int v = 1;
            write(fd[1], &v, sizeof(v));
        }
        kill(pid, SIGUSR1);
        while(1) { 
            sigsuspend(&ss2);
        }
    }
    return 0;
}
