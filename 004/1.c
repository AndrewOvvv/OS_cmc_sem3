#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>


enum 
{
    KBYTE_SIZE = 1024
};


int main(int argc, char *argv[]) 
{
    unsigned long long summary_size = 0;
    for (int i = 1; i < argc; ++i) {
        char *pathname = argv[i];
        struct stat stb;
        if (lstat(pathname, &stb) >= 0 && S_ISREG(stb.st_mode) && stb.st_size % KBYTE_SIZE == 0 && stb.st_nlink == 1) {
            summary_size += stb.st_size;
        }
    }
    printf("%llu\n", summary_size);
    return 0;
}
