#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


void
print_valid_regular_files(int depth, char *path, long long max_size, char *start_path)
{
    if (depth == 0 || !path) {
        return;
    } else {
        DIR *local_dir = opendir(path);
        if (!local_dir) {
            return;
        }
        char new_path[PATH_MAX];
        struct dirent *file_in_dir;
        while ((file_in_dir = readdir(local_dir)) != NULL) {
            char *name = file_in_dir->d_name;
            if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
                continue;
            }
            if (sprintf(new_path, "%s/%s", path, name) >= sizeof(new_path)) {
                continue;
            }
            struct stat stb;
            if (lstat(new_path, &stb) >= 0) {
                if (S_ISREG(stb.st_mode) && access(new_path, R_OK) >= 0 && stb.st_size <= max_size) {
                    printf("%s\n", new_path + strlen(start_path) + 1);
                } else if (S_ISDIR(stb.st_mode)) {    
                    print_valid_regular_files(depth - 1, new_path, max_size, start_path);
                }
            }
        }
        closedir(local_dir);
    }
}


int
main(int argc, char *argv[]) 
{
    if (argc < 3) {
        exit(1);
    }
    
    char path[PATH_MAX];
    if (sprintf(path, "%s", argv[1]) >= sizeof(path)) {
        return 0;
    }

    char *eptr = NULL;
    errno = 0;
    long long max_size = strtoll(argv[2], &eptr, 10);
    if (errno || *eptr || eptr == argv[2] || (long long) max_size != max_size) {
        exit(1);
    }

    int depth = 4;

    print_valid_regular_files(depth, path, max_size, path);
    return 0;
}
