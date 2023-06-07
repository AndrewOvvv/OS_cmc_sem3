#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>


int fd[2];
int pid[2];
int serial;
unsigned long long max_value;


void
handler(int s)
{
    unsigned long long value = 0;

    char current; read(fd[0], &current, sizeof(current));
    while (current != 0) {
        value = 10 * value + (current - '0');
        read(fd[0], &current, sizeof(current));
    }
    read(fd[0], &pid[1 - serial], sizeof(pid[1 - serial]));
    //printf("%d %d %d\n", serial, pid[0], pid[1]);


    if (value == max_value) {
        current = 0;
        write(fd[1], &current, sizeof(current));
        write(fd[1], &pid[serial], sizeof(pid[serial]));
        close(fd[0]);
        close(fd[1]);
        kill(pid[1 - serial], SIGUSR1);
        _exit(0);
    } else if (value == 0) {
        close(fd[0]);
        close(fd[1]);
        _exit(0); 
    } else {
        printf("%d %llu\n", serial + 1, value); fflush(stdout);
        ++value;

        unsigned long long power = 1;
        while (power * 10 <= value) {
            power *= 10;
        }

        while (power > 0) {
            current = '0' + value / power;
            write(fd[1], &current, sizeof(current));
            value = value - (value / power) * power;
            power /= 10;
        }
        current = 0;
        write(fd[1], &current, sizeof(current));
        write(fd[1], &pid[serial], sizeof(pid[serial]));
        kill(pid[1 - serial], SIGUSR1);
    }
}


int
main(int argc, char *argv[])
{
    max_value = strtoll(argv[1], NULL, 10);

    sigset_t ss1, ss2;

    sigemptyset(&ss1);
    sigemptyset(&ss2);
    sigaddset(&ss1, SIGUSR1);
    sigprocmask(SIG_BLOCK, &ss1, NULL);

    pipe(fd);
    sigaction(SIGUSR1, 
            &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART},
            NULL);


    if (!(pid[0] = fork())) {
        // son
        pid[0] = getpid();
        serial = 0;
        while (1) {
            sigsuspend(&ss2);
        }
    } 
    
    if (!(pid[1] = fork())) {
        // son 
        pid[1] = getpid();
        serial = 1;
        {
            char value[2] = {'1', 0};
            write(fd[1], &value, sizeof(value));
            write(fd[1], &pid[1], sizeof(pid[1]));
        }
        kill(pid[0], SIGUSR1);
        while(1) { 
            sigsuspend(&ss2);
        }
    }

    while (wait(NULL) > 0) {}
    printf("Done"); fflush(stdout);
    return 0;
}
