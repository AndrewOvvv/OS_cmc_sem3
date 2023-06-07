#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>


int
main(void )
{
    int fd[2];
    int count = 0;
    
    printf("%d\n", (int) PIPE_BUF);
    
    //pipe2(fd, O_NONBLOCK);
    pipe(fd);
    fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK);

    while (1) {
        char c = 0;
        if (write(fd[1], &c, sizeof(c)) > 0) {
            break;
        }
        ++count;
    }
    printf("%d\n", count);
}
