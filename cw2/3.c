#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>


void
work(int *dst) 
{
    int depth = *dst;
    for (int i = 0; i < depth; ++i) {
        int pid = fork();
        if (pid < 0) {
            _exit(1);
        } else if (pid) {
            wait(NULL);
            if (i != 0) {
                _exit(0);
            }
            return;
        }    
    }
    if (scanf("%d", dst) == EOF) {
        *dst = -1;
    } else {
        printf("%lld %lld\n", (long long)getpid(), (long long)*dst);
        fflush(stdout);
    }
    if (depth) {
        _exit(0);
    }
}


int
main(void ) 
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    int *dst = mmap(0, sizeof(*dst), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if (dst == MAP_FAILED) {
        exit(1);
    }

    *dst = 0;
    while (*dst > -1) {
        work(dst);
    }
    munmap(dst, sizeof(*dst));
    return 0;
}
