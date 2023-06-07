#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


volatile long long last_prime = 0;
volatile int cnt_exec = 0;


long long
max(long long x, long long y)
{
    if (x < y) {
        return y;
    }
    return x;
}


void 
find_prime(long long left, long long right)
{
    for (long long i = max(2, left); i < right; ++i) {
        long long curr_prime = 2, n = i;
        while (curr_prime * curr_prime <= n) {
            if (n % curr_prime == 0) {
                n = -1;
                break;
            }
            ++curr_prime;
        }
        if (n != -1) {
            last_prime = i;
        }
    }
}


void
handler(int signal)
{
    if (signal == SIGINT && cnt_exec == 3) {
        exit(0);
    } else if (signal == SIGINT) {
        printf("%lld\n", last_prime); fflush(stdout);
        ++cnt_exec;
    } else if (signal == SIGTERM) {
        exit(0);
    }
}


int
main(int argc, char *argv[])
{
    sigaction(SIGINT, 
            &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART},
            NULL);
    sigaction(SIGTERM, 
            &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART},
            NULL);

    long long left, right;
    scanf("%lld", &left);
    scanf("%lld", &right);

    pid_t mypid = getpid();
    printf("%lld\n", (long long)mypid); fflush(stdout);

    find_prime(left, right);

    printf("-1\n"); fflush(stdout);
    return 0;
}

