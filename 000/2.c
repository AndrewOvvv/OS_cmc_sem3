#include <stdio.h>
#include <stdlib.h>


enum
{
    MIN_ASCII_LOWCASE = 'a',
    MAX_ASCII_LOWCASE = 'z',
    MIN_ASCII_UPPERCASE = 'A',
    MAX_ASCII_UPPERCASE = 'Z',
    MIN_ASCII_NUMBER = '0',
    MAX_ASCII_NUMBER = '9',
    CNT_NUMBERS = 10,
    CNT_LETTERS = 26,
    CODE_ATSIGN = 0,
    CODE_NUMBER = CODE_ATSIGN + 1,
    CODE_LOWERCASE = CODE_NUMBER + CNT_NUMBERS,
    CODE_UPPERCASE = CODE_LOWERCASE + CNT_LETTERS,
    CODE_NUMSIGN = CODE_UPPERCASE + CNT_LETTERS,
    NULL_CODE = 128
};


unsigned
CharToCode(int c)
{
    if (MIN_ASCII_NUMBER <= c && c <= MAX_ASCII_NUMBER) {
        return CODE_NUMBER + (c - MIN_ASCII_NUMBER);
    } else if (MIN_ASCII_LOWCASE <= c && c <= MAX_ASCII_LOWCASE) {
        return CODE_LOWERCASE + (c - MIN_ASCII_LOWCASE);
    } else if (MIN_ASCII_UPPERCASE <= c && c <= MAX_ASCII_UPPERCASE) {
        return CODE_UPPERCASE + (c - MIN_ASCII_UPPERCASE);
    } else {
        return NULL_CODE;
    }
}


int
CodeToChar(unsigned code)
{
    if (CODE_ATSIGN == code) {
        return '@';
    } else if (CODE_NUMBER <= code && code < CODE_LOWERCASE) {
        return '0' + (code - CODE_NUMBER);
    } else if (CODE_LOWERCASE <= code && code < CODE_UPPERCASE) {
        return 'a' + (code - CODE_LOWERCASE);
    } else if (CODE_UPPERCASE <= code && code < CODE_NUMSIGN) {
        return 'A' + (code - CODE_UPPERCASE);
    } else {
        return '#';
    }
}


int
main(void ) 
{
    int char_in;
    
    while ((char_in = getchar()) != EOF) {
        unsigned cur_code = CharToCode(char_in);
        if (cur_code != NULL_CODE) {
            cur_code = cur_code ^ (1u << 3u);
            cur_code = cur_code & (~(1u << 2u));
            putchar(CodeToChar(cur_code));
        }
    }
    return 0;
}
