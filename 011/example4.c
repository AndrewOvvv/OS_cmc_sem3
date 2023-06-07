#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <limits.h>
#include <stdint.h>
#include <sys/wait.h>


void
work(int32_t serial, int32_t rfd, int32_t wfd) 
{
    while (1) {
        int32_t value;
        read(rfd, &value, sizeof(value));
        //printf("%d %d\n", serial, value);
        ++value;
        printf("%d %d\n", serial, value - 1); fflush(stdout);
        write(wfd, &value, sizeof(value));
        //printf("%d %d\n", serial, value - 1); fflush(stdout); // <--- прооцирует ошибку
    }
}


int
main(void )
{

    int32_t p01[2];
    int32_t p10[2];

    pipe(p01);
    pipe(p10);


    {
        int32_t value = 1;
        write(p10[1], &value, sizeof(value));
    }


    if (!fork()) {
        close(p10[1]); close(p01[0]);
        work(0, p10[0], p01[1]);
        _exit(1);
    }
    close(p10[0]); close(p01[1]);
    if (!fork()) {
        work(1, p01[0], p10[1]);
        _exit(1);
    }
    close(p01[0]); close(p10[1]);

    while (wait(NULL) > 0) {}

}
