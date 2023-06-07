#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 1) {
        return 1;
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);

/*    double v;
    while (scanf("%lf", &v) == 1) {
        write(fd, &v, sizeof(v));
    } */

    unsigned int v;
    while (scanf("%u", &v) == 1) {
        write(fd, &v, sizeof(v));
    }

    close(fd);
    return 0;
}
