#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/select.h>


volatile int intflag = 0;
void handler(int s) 
{
    intflag = 1;
}


int
main(int argc, char *argv[])
{
    mkfifo("fifo", 0600);
    int fdf = open("fifo", O_RDONLY);

    sigset_t ss, ss1;
    sigemptyset(&ss);
    sigemptyset(&ss1);

    sigaddset(&ss, SIGINT);
    sigaction(SIGINT, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);

    struct timespec to = {.tv_sec = 1, .tv_nsec = 500000000 };


    while (1) {
        fd_set rfds;
        FD_ZERO(&rfds);
        int maxfd = 0;

        FD_SET(0, &rfds);
        FD_SET(fdf, &rfds);

        if (fdf > maxfd) {
            maxfd = fdf;
        }
        int r = pselect(maxfd + 1, &rfds, NULL, NULL, &to, &ss1);
        if (r < 0 && errno == EINTR) {
            intflag = 0;
            printf("Signal!\n");
        } else if (r < 0) {
            fprintf(stderr, "error: %s\n", strerror(errno));
            exit(1);
        } else if (r == 0) {
            printf("Timeout!\n");
        } else if (FD_ISSET(0, &rfds)) {
            char buf[80];
            int rr = read(fdf, buf, sizeof(buf) - 1);
            buf[rr] = 0;
            printf("stdin: %s\n", buf);
        } else if (FD_ISSET(fdf, &rfds)) {
            char buf[80];
            int rr = read(fdf, buf, sizeof(buf) - 1);
            buf[rr] = 0;
            printf("pipe: %s\n", buf);
        }
    }
    return 0;
}
