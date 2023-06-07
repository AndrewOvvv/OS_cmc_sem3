#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>


int usr_flag = 0;


void
handler()
{
    usr_flag = 1;
}


void
work(int fd[2], int serial, int pid, sigset_t *ss)
{
    int v;

    while (!usr_flag) {
        sigsuspend(ss);
    }
    usr_flag = 0;

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
    int fd[2];
    int pid;

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
        while (1) {
            work(fd, 2, getppid(), &ss2);
        }
    } else {
        // parent
        {
            int v = 1;
            write(fd[1], &v, sizeof(v));
        }
        kill(pid, SIGUSR1);
        while(1) { 
            work(fd, 1, pid, &ss2);
        }
    }
    return 0;
}
