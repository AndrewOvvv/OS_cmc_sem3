#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <sys/wait.h>


void
work(int serial, int rfd, int wfd, unsigned long long  max_value) 
{
    while (1) {
        unsigned long long value = 0;

        char current; read(rfd, &current, sizeof(current));
        while (current != 0) {
            value = 10 * value + (current - '0');
            read(rfd, &current, sizeof(current));
        }

        if (value == 0) {
            return;
        } else if (value == max_value) {
            current = 0;
            write(wfd, &current, sizeof(current));
            return;
        } else {
            printf("%d %llu\n", serial, value); fflush(stdout);
            ++value;

            unsigned long long power = 1;
            while (power * 10 <= value) {
                power *= 10;
            }

            while (power > 0) {
                current = '0' + value / power;
                write(wfd, &current, sizeof(current));
                value = value - (value / power) * power;
                power /= 10;
            }
            current = 0;
            write(wfd, &current, sizeof(current));
        }
    }
}


int
main(int argc, char *argv[])
{

    int p01[2];
    int p10[2];

    pipe(p01);
    pipe(p10);

    unsigned long long N = strtoll(argv[1], NULL, 10);

    char value[2] = {'1', 0};

    write(p10[1], &value, sizeof(value));

    if (!fork()) {
        close(p10[1]); close(p01[0]);
        work(1, p10[0], p01[1], N);
        close(p10[0]); close(p01[1]);
        _exit(0);
    }
    close(p10[0]); close(p01[1]);


    if (!fork()) {
        work(2, p01[0], p10[1], N);
        close(p01[0]); close(p10[1]);
        _exit(0);
    }
    close(p01[0]); close(p10[1]);

    while (wait(NULL) > 0) {}
    printf("Done"); fflush(stdout);

}
