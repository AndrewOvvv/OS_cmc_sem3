#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int 
main(int argc, char *argv[])
{
    char *num_end;
    long long positive_sum = 0, negative_sum = 0;
    for (int i = 1; i < argc; ++i) {
        long long value = strtoll(argv[i], &num_end, 10);
        if (value < 0) {
            negative_sum += value;
        } else {
            positive_sum += value;
        }
    }
    printf("%lld\n%lld\n", positive_sum, negative_sum);
    return 0;
}
