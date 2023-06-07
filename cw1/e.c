#define _GNU_SOURCE
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

struct queue_item
{
    char* path;
    struct queue_item* next;
};

int main(int argc, const char** argv)
{
    if(argc < 3)
        return 1;
    char* endp;
    uid_t uid = strtoll(argv[2], &endp, 10);
    if(*endp || endp == argv[2])
        return 1;
    struct queue_item* queue = calloc(1, sizeof(*queue));
    queue->path = strdup(argv[1]);
    uintmax_t count = 0;
    while(queue)
    {
        char* path = queue->path;
        struct queue_item* next = queue->next;
        free(queue);
        queue = next;
        int fd = open(path, O_RDONLY | O_NOFOLLOW | O_PATH);
        if(fd < 0)
            continue;
        struct stat st;
        if(fstat(fd, &st))
        {
            close(fd);
            continue;
        }
        if(S_ISREG(st.st_mode) && st.st_uid == uid)
        {
            close(fd);
            count++;
        }
        else if(S_ISDIR(st.st_mode))
        {
            int fd1 = openat(fd, ".", O_RDONLY|O_DIRECTORY);
            close(fd);
            if(fd1 < 0)
                continue;
            DIR* d = fdopendir(fd1);
            if(!d)
                continue;
            struct dirent* de;
            while((de = readdir(d)))
            {
                if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
                    continue;
                size_t l1 = strlen(path);
                size_t l2 = strlen(de->d_name);
                char* path2 = malloc(l1+l2+2);
                memcpy(path2, path, l1);
                path2[l1] = '/';
                memcpy(path2+l1+1, de->d_name, l2+1);
                struct queue_item* head = calloc(1, sizeof(*head));
                head->path = path2;
                head->next = queue;
                queue = head;
            }
            closedir(d);
        }
        free(path);
    }
    printf("%" PRIuMAX "\n", count);
    return 0;
}
