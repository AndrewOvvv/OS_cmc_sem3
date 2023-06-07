#include <stdio.h>
#include <time.h>
#include <sys/time.h>


int
main()
{
    int year, mon, day, hour, minm sec;
    scanf("%d%d%d%d%d%d", &year, &mon, &day, &hour, &min, &sec);

    struct tm t = {};
    t.tm_year = year - 1900;
    t.tm_mon = mon - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = min;
    t.tm_sec = sec;
    t.tm_isdst = -1;

    errno = 0;
    time_t ts = mktime(&t);
    if (errno) {
        fprintf(stderr, "out of range\n");
        return 1;
    }
    printf("%d", ts.tv_sec);
    return 0;
}
