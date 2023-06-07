#include <stdio.h>
#include <string.h>


enum 
{ 
    CNT_LINES = 3,
    MAX_LEN = 128
};


int
main(void )
{
    char in_lines[CNT_LINES][MAX_LEN];
    for (int i = 0; i < CNT_LINES; ++i) {
        fgets(in_lines[i], MAX_LEN, stdin);        
    }
    for (int i = 0; i < CNT_LINES; ++i) {
        if (in_lines[i][strlen(in_lines[i]) - 1] == '\n') {
            in_lines[i][strlen(in_lines[i]) - 1] = 0;
        }
    }
    printf("[Host:%s,Login:%s,Password:%s]\n", in_lines[0], in_lines[1], in_lines[2]);
    return 0;
}
