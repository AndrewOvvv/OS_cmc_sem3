#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int
main(void )
{
    int N;
    scanf("%d", &N);
    for (int i = 1; i <= N; ++i) {
        if (i != N) {
            printf("%d ", i); fflush(stdout);
        } else {
            printf("%d\n", i); fflush(stdout);
        }
        
        pid_t new_son = fork();
        if (new_son) {
            waitpid(new_son, NULL, 0);
            _exit(0);
        }
    }
    return 0;
}
