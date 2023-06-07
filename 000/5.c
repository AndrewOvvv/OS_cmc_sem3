#include <stdio.h>
#include <stdlib.h>


void
print_permutation(int len, int *perm)
{
    for (int i = 0; i < len; ++i) {
        printf("%i", perm[i]);
    }
    printf("\n");
}


void
print_all_permutations(int deep, int n, int *used, int *perm) 
{
    if (deep == n) {
        print_permutation(n, perm);
        return;
    }
    for (int i = 1; i < n + 1; ++i) {
        if (!used[i]) {
            used[i] = 1;
            perm[deep] = i;
            print_all_permutations(deep + 1, n, used, perm);
            used[i] = 0;
        }
    }
}


int
main(void ) 
{
    int n;
    if (scanf("%d", &n) != 1) {
        exit(1);
    }
    if (!(0 < n && n < 10)) {
        exit(1);
    }

    int *used = calloc(sizeof(*used), n + 1);
    if (!used) {
        exit(1);
    }
    int *perm = calloc(sizeof(*perm), n);
    if (!perm) {
        exit(1);
    }
    print_all_permutations(0, n, used, perm);

    free(used);
    free(perm);
    return 0;
}
