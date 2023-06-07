#include <stdio.h>

int get_leaps_in_range(int a, int b)
{
    int ans = 0;
    ans += (b+3)/4 - (a+3)/4;
    ans -= (b+99)/100 - (a+99)/100;
    ans += (b+399)/400 - (a+399)/400;
    return ans;
}

int is_leap(int year)
{
    return get_leaps_in_range(year, year+1);
}

int days_in_year(int year)
{
    return is_leap(year) ? 366 : 365;
}

int get_jan_1_difference(int a, int b)
{
    return ((b - a) + get_leaps_in_range(a, b)) % 7;
}

int get_jan_1_weekday(int year)
{
    return (5 /* saturday */ + get_jan_1_difference(2000, year)) % 7;
}

int n_sundays(int start_day, int n_days)
{
    return (start_day + n_days) / 7;
}

int n_holidays(int start_day, int n_days)
{
    // 8 === 1 (mod 7), thus 2**(n+3)-th day is the same weekday as 2**n
    int ans = n_sundays(start_day, n_days);
    if(start_day != 6)
        ans += 3; //1, 8, 64
    if((start_day + 1) != 6)
        ans += 3; //2, 16, 128
    if((start_day + 3) != 6)
        ans += 3; //4, 32, 256
    return ans;
}

int main(void)
{
    int year;
    scanf("%d", &year);
    int start = get_jan_1_weekday(year);
    printf("%d\n", n_holidays(start, days_in_year(year)));
    return 0;
}
