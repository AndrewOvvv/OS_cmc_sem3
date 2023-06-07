#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int
check_exe_suffix(char *name)
{
    int n = strlen(name);
    return (n >= 4 && name[n - 1] == 'e' && name[n - 2] == 'x' && name[n - 3] == 'e' && name[n - 4] == '.');
}


int
main(int argc, char *argv[]) 
{
    if (argc < 2) {
        exit(1);
    }
    
    unsigned long long count_files = 0;
    DIR *dir = opendir(argv[1]);
    struct dirent *file_dir;
    while ((file_dir = readdir(dir)) != NULL) {
        char *name = file_dir->d_name;
        char path[PATH_MAX];
        if (sprintf(path, "%s/%s", argv[1], name) >= sizeof(path)) {
            continue;
        }
        struct stat stb;
        if (stat(path, &stb) >= 0) {
            if (S_ISREG(stb.st_mode) && (access(path, X_OK) >= 0) && check_exe_suffix(name)) {
                ++count_files;
            }
        }
    }
    printf("%llu\n", count_files);
    closedir(dir);
    return 0;
}
