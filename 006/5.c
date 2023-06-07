#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <stdlib.h>

enum
{
    START_YEAR = 1900, 
    FULL_MOON_YEAR = 2021,
    LOW_YEAR = 1902,
    MONDAY = 1
};


int
main(void )
{
    int current_year; 
    scanf("%d", &current_year);
    
    struct tm t = {};
    t.tm_year = current_year - START_YEAR;
    t.tm_mon = 0;
    t.tm_mday = 1;
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_isdst = -1;
    
    errno = 0;
    mktime(&t);
    if (errno) {
        exit(1);
    }

    t.tm_mday += 256;
    errno = 0;
    mktime(&t);
    if (errno) {
        exit(1);
    }

    struct tm full_moon = {};
    full_moon.tm_year = FULL_MOON_YEAR - START_YEAR;
    full_moon.tm_mon = 4;
    full_moon.tm_mday = 26;
    full_moon.tm_hour = 11;
    full_moon.tm_min = 14;
    full_moon.tm_sec = 0;
    full_moon.tm_isdst = -1;

    errno = 0;
    mktime(&full_moon);
    if (errno) {
        exit(1);
    }
    
    while (full_moon.tm_year > LOW_YEAR - START_YEAR || full_moon.tm_mon > 0 || full_moon.tm_mday > 28) {
        full_moon.tm_mday -= 29;
        full_moon.tm_hour -= 12;
        full_moon.tm_min -= 44;
        errno = 0;
        mktime(&full_moon);
        if (errno) {
            break;
        }
    }
    
    while ((full_moon.tm_year * 10000 + full_moon.tm_mon * 100 + full_moon.tm_mday) < 
            (t.tm_year * 10000 + t.tm_mon * 100 + t.tm_mday)) {
        full_moon.tm_mday += 29;
        full_moon.tm_hour += 12;
        full_moon.tm_min += 44;
        errno = 0;
        mktime(&full_moon);
        if (errno) {
            break;
        }
    }
    
    full_moon.tm_mday += 1;
    errno = 0;
    mktime(&full_moon);
    if (errno) {
        exit(1);
    }    

    int cnt_mon = 0;
    while (cnt_mon < 4) {
        if (full_moon.tm_wday == MONDAY) {
            cnt_mon++;
        }
        if (cnt_mon == 4) {
            printf("%d-%02d-%02d\n", full_moon.tm_year + 1900, full_moon.tm_mon + 1, full_moon.tm_mday);
        }
        
        full_moon.tm_mday += 1;
        errno = 0;
        mktime(&full_moon);
        if (errno) {
            break;
        }
    }
    return 0;
}
