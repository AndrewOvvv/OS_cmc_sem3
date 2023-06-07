#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>


int
main(void )
{
    printf("hello\n"); // if stdout in terminal it will write "hello\n", if we write stdout in file it will write "hello\nhello\n"
    pid_t pid = fork();
    if (pid < 0) {
        exit(1);
    }
    return 0;
}
