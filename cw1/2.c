#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>


static int
compare_uint64(const void* a, const void* b)
{
    uint64_t first = *(const uint64_t*)a;
    uint64_t second = *(const uint64_t*)b;
    if (first > second) {
        return -1;
    }
    if (first < second) {
        return 1;
    }
    return 0;
}


int
main(int argc, const char** argv)
{
    uint64_t *mem = calloc(argc ? argc - 1 : 0, sizeof(*mem));
    if (!mem) {
        exit(1);
    }
    for (int i = 1; i < argc; i++) {
        char* endp;
        mem[i - 1] = strtoull(argv[i], &endp, 17);
        if (!(argv[i] != endp && !*endp)) {
            exit(1);        
        }
    }
    qsort(mem, argc ? argc - 1 : 0, sizeof(*mem), compare_uint64);
    for (int i = 1; i < argc; i++) {
        printf("%" PRIu64 "\n", mem[i - 1]);
    }
    return 0;
}
