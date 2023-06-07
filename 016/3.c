#include <stdio.h>
#include <stdlib.h>


enum { BORDERS = 2 };


unsigned
get_size(unsigned *FAT, unsigned start, unsigned N)
{
    unsigned current = start;
    unsigned size = 0;
    while (current != 0 && current != N - 1 && current != N - 2) {
        ++size;
        current = FAT[current];
    }
    return size;
}


int
main(void )
{
    unsigned N; 
    scanf("%u", &N);

    unsigned *FAT = calloc(N, sizeof(*FAT));
    if (FAT == NULL) {
        exit(1);
    }

    for (int i = BORDERS; i < N - BORDERS; ++i) {
        scanf("%u", &FAT[i]);
    }

    unsigned start = -1;
    while (scanf("%u", &start) != EOF) {
        printf("%u\n", get_size(FAT, start, N));
    }

    free(FAT);
    return 0;
}
