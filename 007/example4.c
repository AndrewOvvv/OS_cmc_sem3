#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int
main(void )
{
    for (int i = 0; i < 20; ++i) {
        if (!fork()) {
            printf("child %d\n", i);
            fflush(stdout);
            _exit(0);
        }
    }
    
    while (wait(NULL) > 0) {}
    return 0;
}
