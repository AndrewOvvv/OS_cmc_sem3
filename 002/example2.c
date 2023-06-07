#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>


int main()
{
    uint8_t c;
    
    while (1) {
        ssize_t r = read(STDIN_FILENO, &c, sizeof(c));
        if (r < 0) {
            fprintf(stderr, "read failed: %s\n", stderror(errno));
            _exit(1);
        }
        if (!r) {
            break;
        }
        r = write(STDOUT_FILENO, &c, sizeof(c));
        if (r < 0) {
            fprintf(stderr, "write failed: %s\n", stderror(errno));
            _exit(1);
        }
    }
    
    _exit(0);
}
