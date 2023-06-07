#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>


enum
{
    PLUS = '+',
    MINUS = '-'
};


int verified_multiply(int a, int b)
{
    long long a_ll = a, b_ll = b;
    a_ll *= b_ll;
    if (INT_MIN <= a_ll && a_ll <= INT_MAX) {
        return (int)a_ll;
    } else {
        return 0;
    }
}


void
get_value(long long *result, char *buf)
{
    int len = strlen(buf), multiply_k = 0, operation = 1;
    //get sign of number
    if (len == 0 || (buf[len - 1] != PLUS && buf[len - 1] != MINUS)) {
        return;
    } else if (buf[len - 1] == MINUS) {
        operation = -1;
    }
    buf[len - 1] = 0;

    //check k
    if (1 < len && buf[len - 2] == 'k') {
        multiply_k = 1;
        buf[len - 2] = 0;
    }
    
    //strtol with start of string
    char *eptr = NULL;
    errno = 0;
    long N = strtol(buf, &eptr, 10);
    if (errno || *eptr || eptr == buf || (int) N != N) {
        return;
    }

    //if multiply_k mul N and 1000 and check overflow
    int local_result = N;
    if (multiply_k) {
        local_result = verified_multiply(local_result, 1000);
    }

    //change result
    if (operation == 1) {
        *result += local_result;
    } else {
        *result -= local_result;
    }
}


int
main(int argc, char *argv[])
{
    long long result = 0;
    for (int i = 1; i < argc; ++i) {
        get_value(&result, argv[i]);
    }
    printf("%lld\n", result);
    return 0;
}
