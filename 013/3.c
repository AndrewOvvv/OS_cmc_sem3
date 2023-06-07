#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int
main(int argc, char *argv[])
{
    int cnt_proc = 1;
    int pipe_array[2];
    pipe(pipe_array);

    int fd_file1 = open(argv[4], O_RDONLY, 0666);
    int fd_file2 = open(argv[5], O_WRONLY | O_CREAT | O_APPEND, 0666);
    
    pid_t son3 = fork();
    if (!son3) {
        // son
        dup2(pipe_array[0], STDIN_FILENO);
        dup2(fd_file2, STDOUT_FILENO);

        close(fd_file1);
        close(fd_file2);

        close(pipe_array[0]);
        close(pipe_array[1]);
        
        execlp(argv[3], argv[3], NULL);
        _exit(1);
    }

    close(fd_file2);
    close(pipe_array[0]);

    pid_t son1 = fork();
    if (!son1) {
        // son
        dup2(fd_file1, STDIN_FILENO);
        dup2(pipe_array[1], STDOUT_FILENO);

        close(fd_file1);

        close(pipe_array[0]);
        close(pipe_array[1]);

        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }

    close(fd_file1);
 

    int status;
    waitpid(son1, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        ++cnt_proc;
        pid_t son2 = fork();
        if (!son2) {
            // son
            dup2(pipe_array[1], STDOUT_FILENO);

            close(pipe_array[0]);
            close(pipe_array[1]);

            execlp(argv[2], argv[2], NULL);
            _exit(1);
        }
    }

    close(pipe_array[1]);

    for (int i = 0; i < cnt_proc; ++i) {
        wait(NULL);
    } 
    return 0;
}
