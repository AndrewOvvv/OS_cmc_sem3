#include <stdio.h>
#include <time.h>
#include <sys/time.h>


int
main()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("%ld.%06ld\n", tv.tv_sec, tv.tv_usec);
    struct tm lt;
    localtime_r(&tv.tv_sec, &lt);
    printf("%04d-%02d-%02d %02d:%02d:%02d %d %d %d\n",
        lt.tm_year, lt.tm_mon, lt.tm_mday, 
        lt.tm_hour, lt.tm_min, lt.tm_sec,
        lt.tm_wday, lt.tm_zone, lt.tm_isdst);
    return 0;
}
