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


int
main(int argc, char *argv[]) 
{
    if (argc < 3) {
        exit(1);
    }

    unsigned long long summary_size = 0;
    char fpath[PATH_MAX];
    char spath[PATH_MAX];
    DIR *first_dir = opendir(argv[1]);

    struct dirent *file_in_dir;
    while ((file_in_dir = readdir(first_dir)) != NULL) {
        char *name = file_in_dir->d_name;
        if (sprintf(fpath, "%s/%s", argv[1], name) >= sizeof(fpath)) {
            continue;
        }
        
        struct stat stb;
        if (lstat(fpath, &stb) >= 0 && S_ISREG(stb.st_mode) && access(fpath, W_OK) == 0) {
            int was_link = 0;
            DIR *second_dir = opendir(argv[2]); 
            struct dirent *sfile_in_dir;
            while ((sfile_in_dir = readdir(second_dir)) != NULL) {
                char *sname = sfile_in_dir->d_name;
                if (sprintf(spath, "%s/%s", argv[2], sname) >= sizeof(spath)) {
                    continue;
                }

                struct stat sstb;
                if (stat(spath, &sstb) >= 0 && stb.st_ino == sstb.st_ino && stb.st_dev == sstb.st_dev) {
                    was_link = 1;
                }
            }
            closedir(second_dir);

            if (was_link) {
                summary_size += stb.st_size;            
            }
        }        
    }
    closedir(first_dir);
    
    printf("%llu\n", summary_size);
    return 0;
}
