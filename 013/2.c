#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>


int32_t
main(void )
{
    int32_t current;
    int32_t pipe_array[2];
    pipe(pipe_array);

    pid_t son = fork();
    if (!son) {
        pid_t gson = fork();
        if (!gson) {
            // grandson
            int32_t addition;
            int64_t sum = 0;
            while (read(pipe_array[0], &addition, sizeof(addition))) {
                sum += addition;       
            }
            close(pipe_array[0]);
            close(pipe_array[1]);
            printf("%ld\n", sum); fflush(stdout);
            _exit(0);
        } else {    
            // son
            close(pipe_array[0]);
            close(pipe_array[1]);
            waitpid(gson, NULL, 0);
            _exit(0);
        }
    } else {
        // parent
        while (scanf("%d", &current) == 1) {
            write(pipe_array[1], &current, sizeof(current));
        }
        close(pipe_array[0]);
        close(pipe_array[1]);
        waitpid(son, NULL, 0);
    }
    return 0;
}
