#include <stdio.h>
#include <stdlib.h>


int
comp(const void* elem1, const void* elem2) 
{
    int a = *(const int*)elem1;
    int b = *(const int*)elem2;
    return a > b;
}


void
sort_even_odd(size_t count, int *data)
{
    int cnt_even = 0, cnt_odd = 0;
    for (int i = 0; i < count; ++i) {
        if (data[i] & 1) {
            cnt_odd++;
        } else {
            cnt_even++;
        }
    }

    int *even_data = calloc(sizeof(*even_data), cnt_even);
    int *odd_data = calloc(sizeof(*odd_data), cnt_odd);
    
    
    cnt_even = 0;
    cnt_odd = 0;

    for (int i = 0; i < count; ++i) {
        if (data[i] & 1) {
            odd_data[cnt_odd] = data[i];
            cnt_odd++;
        } else {
            even_data[cnt_even] = data[i];
            cnt_even++;
        }
    }

    qsort(even_data, cnt_even, sizeof(*even_data), comp);
    qsort(odd_data, cnt_odd, sizeof(*odd_data), comp);
    
    int lst_data = 0;
    for (int i = 0; i < cnt_even; ++i) {
        data[lst_data] = even_data[i];
        lst_data++;
    }
    for (int i = cnt_odd - 1; i > -1; --i) {
        data[lst_data] = odd_data[i];
        lst_data++;
    }
}
