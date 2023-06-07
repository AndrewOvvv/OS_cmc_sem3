#include <stdio.h>
#include <stdlib.h>
#include <string.h>


long long
satsum(long long v1, long long v2)
{
    if (v1 > 0 && v2 > 0 && LLONG_MAX - v1 < v2) {
        return MY_INT_MAX;
    } else if (v1 < 0 && v2 < 0 && LLONG_MIN - v1 > v2) {
        return MY_INT_;
    } else {
        return v1 + v2;
    }
}


int
main(void )
{   
    char current_char;
    long long value = 0;
    int was_before = 0, over = 0;
    while (scanf("%c", &current_char) != EOF) {
        if (was_before && !is_sym(current_char)) {
            if (over) {
                printf("%lld\n", LLONG_MAX);
            } else {
                printf("%lld\n", value);
            }
        } else if (is_sym(current_char)) {
            int previous_value = value;
            value *= 3;
            if (value / 3 != previous_value) {
                over = 1;
            }
            
        }
    }
    return 0;
}
