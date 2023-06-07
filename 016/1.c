#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int
main(int argc, char *argv[])
{
    if (argc < 3) {
        exit(1);
    }

    long long cache_size, block_size;
    long long cache_blocks;
    long long cnt_misses = 0;
    cache_size = strtol(argv[2], NULL, 10);
    block_size = strtol(argv[3], NULL, 10);
    
    cache_blocks = cache_size / block_size;

    long long *cache = calloc(cache_blocks, sizeof(*cache));
    for (int i = 0; i < cache_blocks; ++i) {
        cache[i] = -1;
    }


    char type_operation, work_with;
    long long addr, operation_size;
    int operation_value;

    while (scanf("%c", &type_operation) != EOF) {
        scanf("%c", &work_with);
        scanf("%c", &type_operation);
        scanf("%llx", &addr);
        scanf("%lld", &operation_size);
        scanf("%d", &operation_value);

        long long memory_block_num = (addr / block_size);
        long long  cache_block_num = memory_block_num % cache_blocks;
        if (cache[cache_block_num] != memory_block_num) {
            if (cache[cache_block_num] != -1) {
                ++cnt_misses;
            }
            cache[cache_block_num] = memory_block_num;
        }
    } 

    printf("%lld\n", cnt_misses);

    free(cache);
    return 0;
}
