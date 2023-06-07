#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>


volatile int bit_number = 0;
volatile int current_bit = -1;
volatile int sons_pid[2];
volatile int correct = 0;


void
handler(int signal)
{
    if (sons_pid[0] != 0 && sons_pid[1] != 0) {
        return;
    } else if (sons_pid[0] != 0) {
        // son 1
        if (signal == SIGUSR1) {
            current_bit = 0;
            correct = 1;
        } else if (signal == SIGUSR2) {
            current_bit = 1;
            correct = 1;
        } else if (signal == SIGIO) {
            _exit(0);
        }
    } else {
        // son 2
        if (signal == SIGALRM) {
            correct = 1;
            return;
        }
    }
}


int
main(int argc, char *argv[])
{
    char *filename = argv[1];

    sigset_t ss1, ss2;

    sigemptyset(&ss1);
    sigemptyset(&ss2);
    //sigaddset(&ss2, SIGALRM);

    //sigaddset(&ss1, SIGUSR1);
    //sigaddset(&ss1, SIGUSR2);
    //sigaddset(&ss1, SIGIO);


    sigaction(SIGUSR1,
            &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART},
            NULL);
    sigaction(SIGUSR2,
            &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART},
            NULL);
    sigaction(SIGIO,
            &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART},
            NULL);
    sigaction(SIGALRM,
            &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART},
            NULL);

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    sons_pid[0] = 1;
    sons_pid[1] = 1;
    // son 1
    if (!(sons_pid[0] = fork())) {
        signal(SIGALRM, SIG_IGN);

        sons_pid[0] = 1;
        sons_pid[1] = 0;
        unsigned char value = 0;

        while (1) {
            while (!correct) {
                pause();
            }
            correct = 0;
            if (current_bit) {
                value = (value | (1 << bit_number));
            } 
            ++bit_number;

            if (bit_number == 8) {
                printf("%c", (char)value); fflush(stdout);
                bit_number = 0;
                value = 0;
            }
            kill(0, SIGALRM);
        }
    }


    // son 2
    if (!(sons_pid[1] = fork())) {
        signal(SIGUSR1, SIG_IGN);
        signal(SIGUSR2, SIG_IGN);
        signal(SIGIO, SIG_IGN);

        sons_pid[0] = 0;
        sons_pid[1] = 1;
        
        int bit_readed = 0;
        int fd = open(filename, O_RDONLY);
        int max_bit = lseek(fd, 0, SEEK_END);
        max_bit *= 8;
        lseek(fd, 0, SEEK_SET);
        correct = 1;
        while (1) {
            correct = 0;
            if (bit_readed == max_bit) {
                kill(0, SIGIO);
                close(fd);
                _exit(0);
            }

            lseek(fd, bit_readed / CHAR_BIT, SEEK_SET);
            unsigned char readed;
            read(fd, &readed, sizeof(readed));
            
            if ((readed & (1 << (bit_readed % 8)))) {
                kill(0, SIGUSR2);
            } else {
                kill(0, SIGUSR1);
            }
            ++bit_readed;
            while (!correct) {
                pause();
            }
        }
    }


    while (wait(NULL) > 0) {}
    return 0;
}
