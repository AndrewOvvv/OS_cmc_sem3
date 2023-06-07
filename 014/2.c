#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


int operation_type = 0;


void
handler(int signal)
{
    if (signal == SIGINT) {
        operation_type = 0;
    } else if (signal == SIGQUIT) {
        operation_type = 1;
    }
}



int
main(void )
{
    sigaction(SIGINT, 
            &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART},
            NULL);
    sigaction(SIGQUIT, 
            &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART},
            NULL);

    pid_t mypid = getpid();
    printf("%lld\n", (long long)mypid); fflush(stdout);

    
    unsigned int current_num = 0, current_res = 0;
    while ((scanf("%d", &current_num)) != EOF) {
        if (operation_type == 0) {
            current_res += current_num;
        } else {
            current_res *= current_num;
        }
        printf("%d\n", (int)current_res); fflush(stdout);
    }
    return 0;
}
