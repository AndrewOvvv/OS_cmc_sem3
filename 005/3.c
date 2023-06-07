#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

enum
{
    START_YEAR = 1900, 
    THURSDAY = 4
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
            
    int cnt_thur_in_month = 0;
    int curr_month = -1;
    mktime(&t);

    while (t.tm_year == current_year - START_YEAR) {
        if (curr_month != t.tm_mon) {
            curr_month = t.tm_mon;
            cnt_thur_in_month = 0;
        }
        if (t.tm_wday == THURSDAY) {
            ++cnt_thur_in_month;
        }
        if (t.tm_wday == THURSDAY && (cnt_thur_in_month == 2 || cnt_thur_in_month == 4) && (t.tm_mday % 3 != 0)) {
            printf("%d %d\n", t.tm_mon + 1, t.tm_mday);
        }
        t.tm_mday += 1;
        errno = 0;
        mktime(&t);
        if (errno) {
            break;
        }
    }
    return 0;
}
