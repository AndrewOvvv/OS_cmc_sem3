#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int
main(void )
{
    uint64_t answer = 0;
    int64_t current_number;
    while (scanf("%" PRId64, &current_number) == 1) {
        answer += (unsigned long long)current_number;
    }
    printf("%" PRId64 "\n", ()answer);
}
