#include <stdio.h>


unsigned long long
max(unsigned long long x, unsigned long long y)
{
    if (x < y) {
        return y;
    }
    return x;
}


int
main(void )
{
    unsigned long long last, allignment = 1, size = 0;
    while (scanf("%llu", &last) != EOF) {
        unsigned long long local_allignment = 1;
        while (last % (local_allignment << 1) == 0) {
            local_allignment = (local_allignment << 1);
        }
        allignment = max(allignment, local_allignment);

        if (size % local_allignment != 0) {
            size += (local_allignment - (size % local_allignment));
        }
        size += last;
    }
    
    if (size % allignment != 0) {
        size += (allignment - (size % allignment));
    }  
    if (size == 0) {
        size = 1;
        allignment = 1;
    } 
    printf("%llu %llu\n", size, allignment);
    return 0;
}


