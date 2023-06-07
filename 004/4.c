#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>


int
cmp(const void *f_str, const void *s_str)
{
    const char *arg1 = *(const char **)f_str, *arg2 = *(const char **)s_str;
    return strcmp(arg1, arg2);
}


int main(int argc, char *argv[]) 
{
    qsort(argv + 1, argc - 1, sizeof(char*), cmp);
    int *used = calloc(argc, sizeof(*used));
    for (int i = 1; i < argc; ++i) {
        used[i] = 1;
    }
    for (int i = 1; i < argc; ++i) {
        struct stat stb;
        if (stat(argv[i], &stb) >= 0) {
            for (int j = i + 1; j < argc && used[i]; ++j) {
                struct stat stb_cmp;
                if (stat(argv[j], &stb_cmp) >= 0) {
                    if (stb.st_ino == stb_cmp.st_ino) {
                        used[i] = 0;
                    }
                }
            }
        } else {
            used[i] = 0;
        }        
    }
    
    for (int i = 1; i < argc; ++i) {
        if (used[i]) {
            printf("%s\n", argv[i]);
        }
    }
    
    free(used);
    return 0;
}
