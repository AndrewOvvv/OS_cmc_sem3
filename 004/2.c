#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>


enum
{
    BLOCK_BIT_SIZE = 3
};


int main(int argc, char *argv[]) 
{
    char letters[] = {'x', 'w', 'r'};
    for (int i = 1; i < argc; ++i) {
        unsigned int value = 0;
        if (sscanf(argv[i], "%o", &value) == EOF) {
            exit(1); //cant read argument - strange
        }
        for (int block = 2; block > -1; --block) {
            for (int right = 2; right > -1; --right) {
                if ((value & ((1u << (block * BLOCK_BIT_SIZE)) << right))) {
                    printf("%c", letters[right]);
                } else {
                    printf("-");
                }
            }
        }
        printf("\n");
    }
    return 0;
}
