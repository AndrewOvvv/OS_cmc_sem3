#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>


int
main(int argc, char *argv[])
{
    pid_t son_pid = fork();
    if (son_pid < 0) {
        exit(0);
    } else if (!son_pid) {
        if (argc < 5) {
            _exit(42);
        }
        int fileINd = open(argv[2], O_RDONLY, 0);
        if (fileINd < 0) {
            _exit(42);
        }

        int fileOUTd = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0660);
        if (fileOUTd < 0) {
            _exit(42);
        }

        int fileERRd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0660);
        if (fileERRd < 0) {
            _exit(42);
        }

        if (dup2(fileINd, STDIN_FILENO) < 0) {
            _exit(42);
        }
        if (dup2(fileOUTd, STDOUT_FILENO) < 0) {
            _exit(42);
        }
        if (dup2(fileERRd, STDERR_FILENO) < 0) {
            _exit(42);
        }
        close(fileINd);
        close(fileOUTd);
        close(fileERRd); 

        execlp(argv[1], argv[1], NULL);
        _exit(42);
    }

    int status;
    if (wait(&status) < 0) {
        exit(0);
    }
    printf("%d\n", status);
    return 0;
}
