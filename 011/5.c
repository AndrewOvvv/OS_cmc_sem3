#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int
print_reversed(int depth)
{
    int current, status;
    if (scanf("%d", &current) == EOF) {
        return 0;
    }
    
    pid_t son = fork();
    if (son < 0) {
        printf("-1\n"); fflush(stdout);
        if (depth == 0) {
            return 0;
        } else {
            _exit(1);
        }
    } else if (!son) {
        //in son
        print_reversed(depth + 1);
        _exit(0);
    }
    waitpid(son, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        printf("%d\n", current); fflush(stdout);
        _exit(0);
    }
    if (depth == 0) {
        return 0;
    } else {
        _exit(1);
    }
}

int
main(void )
{
    print_reversed(0);
    return 0;
}
