#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
    DIR *d = opendir(argv[1]);
    struct dirent dd;
    while ((dd = readdir(d)) != NULL) {
        char *name = dd->d_name;
        char path[PATH_MAX];
        if (snprintf(path, sizeof(path), "%s/%s", dir, name) >= sizeof(path)) {
            fprintf(stderr, "path is too long\n");
            continue;
        }
        //asprintf(&path, "%s/%s", dir, name); - сам выделяет динамическую память malloc'ом
        struct stat stb;
        if (lstat(name, &stb) >= 0) {
            printf("dev: %lx; ino: %lu; type: %x; links: %ld; name: %s\n", 
                stb.st_dev, stb.st_ino, (stb.st_mode & S_IFMT), 
                stb.st_nlink, name);
            if (S_ISDIR(stb.st_mode)) {
                printf("directory\n");                
            } else if (S_ISLNK(stb.st_mode)) {
                printf("symlink\n");
            } else if (S_ISREG(stb.st_mode)) {
                printf("file\n");
            }
        }
        //.... do something
        
        free(path);
    }
    closedir(d);
    return 0;
}
