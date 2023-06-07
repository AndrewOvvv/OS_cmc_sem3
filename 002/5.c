#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>


enum 
{
    MAX_LEN_STR = 14
};


struct Elem
{
    struct Elem *next;
    char *str;
};


struct Elem *
create_new_Elem(int value) 
{ 
    struct Elem *new_vertex = calloc(1, sizeof(*new_vertex));
    if (!new_vertex) {
        exit(1);
    }

    char *str = calloc(MAX_LEN_STR, sizeof(*str));
    if (!str) {
        exit(1);
    }
    
    if (sprintf(str, "%d", value) == 0) {
        exit(1);
    }

    new_vertex->next = NULL;
    new_vertex->str = str;
    return new_vertex;
}


struct Elem *
dup_elem(struct Elem *head)
{
    struct Elem *prev = NULL;
    struct Elem *current = head;
    while (current) {
        char *eptr = NULL;
        errno = 0;
        long value = strtol(current->str, &eptr, 10);
        if (!(errno || *eptr || eptr == current->str || (int) value != value || value == INT_MAX)) {
            struct Elem *new_vertex = create_new_Elem(value + 1);
            if (!prev) {
                head = new_vertex;
            } else {
                prev->next = new_vertex;
            }
            
            new_vertex->next = current;
        }
         
        prev = current;
        current = current->next;
    }
    return head;
}
