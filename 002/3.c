#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


const double MAX_PERCENTAGE = 100.;


int
main(int argc, char *argv[])
{
    double current = 0;
    char *num_end;
    int read_start = 0;
    for (int i = 1; i < argc; ++i) {
        if (!read_start) {
            current = strtod(argv[i], &num_end);
            read_start = 1;
        } else {
            double diff = strtod(argv[i], &num_end);
            current = (current * (MAX_PERCENTAGE + diff)) / MAX_PERCENTAGE;
        }
        double fake_current = round(current * 10000.);
        current = fake_current / 10000.;
    }
    printf("%.4lf\n", current);
    return 0;
}
