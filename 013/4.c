#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int32_t
main(int32_t argc, char *argv[])
{
    int output_input = STDIN_FILENO, pid_size = 0, status;
    close(STDERR_FILENO);

    int pipe_array[2];
    int pid_array[argc];

    for (int i = 1; i < argc - 1; ++i) {
        if (pipe(pipe_array) < 0) {
            // error

            for (int i = 0; i < pid_size; ++i) {
                kill(pid_array[i], SIGKILL);
            }
            for (int i = 0; i < pid_size; ++i) {
                wait(&status);
            }
            exit(1);
        }
        

        pid_t son_pid = fork();
        if (son_pid < 0) {
            // error

            for (int i = 0; i < pid_size; ++i) {
                kill(pid_array[i], SIGKILL);
            }

            exit(1);
        } else if (!son_pid) {
            // son
            close(pipe_array[0]);
            
            if (output_input != STDIN_FILENO) {
                dup2(output_input, STDIN_FILENO);
                close(output_input);
            }

            dup2(pipe_array[1], STDOUT_FILENO);
            close(pipe_array[1]);

            execlp(argv[i], argv[i], NULL);
            _exit(42);
        } else {
            // main
            pid_array[pid_size] = son_pid;
            ++pid_size;

            close(output_input);
            close(pipe_array[1]);
            output_input = pipe_array[0];
        }
        
    }
    if (argc > 1) {
        pid_t son_pid = fork();
        if (son_pid < 0) {
            // error

            for (int i = 0; i < pid_size; ++i) {
                kill(pid_array[i], SIGKILL);
            }
            for (int i = 0; i < pid_size; ++i) {
                wait(&status);
            }


            exit(1);
        } else if (!son_pid) {
            // son
            if (output_input != STDIN_FILENO) {
                dup2(output_input, STDIN_FILENO);
                close(output_input);
            }

            execlp(argv[argc - 1], argv[argc - 1], NULL);
            _exit(42);
        }
        pid_array[pid_size] = son_pid;
        ++pid_size;
        close(output_input);
    }
    
    for (int i = 0; i < pid_size; ++i) {
        wait(NULL);
    }
    return 0;
}
