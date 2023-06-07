#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


enum
{
    CNT_SONS = 3
};


void
work(int32_t serial)
{
    char buf[9] = {};
    read(STDIN_FILENO, buf, sizeof(buf) - 1);
    char *eptr = NULL;
    errno = 0;
    int value = strtol(buf, &eptr, 10);
    value *= value;
    printf("%d %d\n", serial, value); fflush(stdout);
    return;
}


int
main(void )
{
    pid_t sons[CNT_SONS];
    for (int32_t i = 0; i < CNT_SONS; ++i) {
        sons[i] = fork();
        if (!sons[i]) {
            work(i + 1);
            _exit(0);
        }
        wait(NULL);
    }
    return 0;
}
