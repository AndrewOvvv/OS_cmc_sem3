#include <stdio.h>
#include <stdlib.h>


int
comp(const void* elem1, const void* elem2) 
{
    int a = *(const int*)elem1;
    int b = *(const int*)elem2;
    if ((a & 1) == (b & 1)) {
        if ((a & 1) == 0) {
            return a > b;
        } else {
            return a < b;
        }
    } else {
        return (a & 1) > (b & 1);
    }
}


void
sort_even_odd(size_t count, int *data)
{
    qsort(data, count, sizeof(*data), comp);
}
