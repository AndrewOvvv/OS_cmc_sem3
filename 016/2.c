#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


enum { ADDR = 3 };



int64_t 
max(int64_t x, int64_t y)
{
    if (x < y) {
        return y;
    }
    return x;
}


int64_t
min(int64_t x, int64_t y)
{
    if (x < y) {
        return x;
    }
    return y;
}


int64_t
max_addr(int64_t size) 
{
    if ((size << 3) > 60) {
        return -1;
    } else {
        return (1ll << (size << 3));
    }
}


int64_t 
calc_max_size(int64_t block_size, int64_t block_num_size, int64_t inode_direct_block_count)
{
    long long count_size = 0;
    int64_t max_block_addr = max_addr(block_num_size);
    if (__builtin_saddll_overflow(count_size, inode_direct_block_count, &count_size)) {
        if (max_block_addr == -1) {
            return -1;
        } else {
            count_size = (long long)max_block_addr;
        }
    }
    int64_t array[ADDR];
    for (int i = 0; i < ADDR; ++i) {
        array[i] = 1;
    }
    for (int i = 0; i < ADDR; ++i) {
        if (__builtin_smulll_overflow(array[max(i - 1, 0)], block_size / block_num_size, (long long *)&array[i])) {
            if (max_block_addr == -1) {
                return -1;
            } else {
                count_size = (long long)max_block_addr;
            }
        }
    }
    for (int i = 0; i < ADDR; ++i) {
        if (__builtin_saddll_overflow(array[i], count_size, &count_size)) {
            if (max_block_addr == -1) {
                return -1;
            } else {
                count_size = (long long)max_block_addr;
            }
        }
    }
    //printf("count_size - %lld || max_block_addr = %ld\n", count_size, max_block_addr);
    if (max_block_addr != -1) {
        count_size = min(count_size, max_block_addr);
    }
    if (__builtin_smulll_overflow(count_size, block_size, &count_size)) {
        return -1;
    }
    return count_size;
}


int
main(void )
{
    long long block_size, block_num_size, inode_direct_block_count; 
    scanf("%lld %lld %lld", &block_size, &block_num_size, &inode_direct_block_count);

    int64_t best_result = 0, current_result = 0;
    int64_t best_block_num_size = 0, current_block_num_size = 1;
    
    while (current_block_num_size <= min(8, block_size)) {
        current_result = calc_max_size(block_size, current_block_num_size, inode_direct_block_count);
        if ((current_result > best_result && best_result != -1) || (current_result == -1 && best_result != -1)) {
            best_result = current_result;
            best_block_num_size = current_block_num_size;
        }
        ++current_block_num_size;
    }

    printf("%lld\n", (long long)calc_max_size(block_size, block_num_size, inode_direct_block_count));
    printf("%lld %lld\n", (long long)best_block_num_size, (long long)best_result);
    return 0;
}
