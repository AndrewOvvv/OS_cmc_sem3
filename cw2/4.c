#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int
work_13(const char* cmd1, const char* file, const char* cmd2)
{
    int pipe_array[2];
    if (pipe(pipe_array)) {
        return 1;
    }
    int pid1 = fork();
    if (!pid1) {
        close(pipe_array[0]);
        if (dup2(pipe_array[1], 1) != 1) {
            close(pipe_array[1]);
            _exit(127);
        }
        close(pipe_array[1]);
        int fd = open(file, O_RDONLY);
        if (fd < 0) {
            _exit(127);
        }
        if (dup2(fd, 0) != 0) {
            close(fd);
            _exit(127);
        }
        close(fd);
        execlp(cmd1, cmd1, NULL);
        _exit(127);
    }
    pid_t pid2 = fork();
    if (!pid2) {
        close(pipe_array[1]);
        if (dup2(pipe_array[0], 0) != 0) {
            close(pipe_array[0]);
            _exit(127);
        }
        close(pipe_array[0]);
        execlp(cmd2, cmd2, NULL);
        _exit(127);
    }
    if (pid1 > 0) {
        waitpid(pid1, NULL, 0);
    }
    if (pid2 < 0) {
        return 127;
    }
    close(pipe_array[0]);
    close(pipe_array[1]);
    int status;
    waitpid(pid2, &status, 0);
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    } else {
        return 127;
    }
}

int
work_4(const char* cmd)
{
    pid_t pid = fork();
    if (!pid)
    {
        execlp(cmd, cmd, NULL);
        _exit(127);
    }
    if (pid < 0) {
        return 127;
    }
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    } else {
        return 127;
    }
}

int
main(int argc, char *argv[])
{
    // насколько мне известно 127 - код выхода для shell
    if (argc < 5) {
        exit(1);
    }

    int status = work_13(argv[1], argv[2], argv[3]);
    if (status) {
        return status;
    } else {
        return work_4(argv[4]);
    }
}
