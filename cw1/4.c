#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


enum
{
    SUNDAY = 0,
    SATURDAY = 6,
    START_YEAR = 1900
};


int
main(void )
{
    unsigned year, day_number = 1, cnt_holydays = 0;
    scanf("%u", &year);
    
    unsigned new_holyday = 1;

    struct tm t = {};
    
    t.tm_year = year - START_YEAR;
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

    while (year - START_YEAR == t.tm_year) {
        printf("%d %d\n", t.tm_mon, t.tm_mday);
        if (day_number == new_holyday) {
            ++cnt_holydays;
            new_holyday *= 2;
        } else if (t.tm_wday == SUNDAY || t.tm_wday == SATURDAY) {
            ++cnt_holydays;
        }

        ++day_number;
        t.tm_mday += 1;
        errno = 0;
        mktime(&t);
        if (errno) {
            break;
        }
    }

    printf("%u\n", cnt_holydays);
    return 0;
}
