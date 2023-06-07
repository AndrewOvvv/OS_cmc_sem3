#include <stdio.h>
#include <stdlib.h>


const double LEFT_B = 2;
const double RIGHT_B = 5;
const double UP_B = 7;
const double DOWN_B = 1;
const double DIFF_COORD = 2;


int
main(void ) 
{
    double px, py;
    int in_polygon = 1;
    if (scanf("%lf", &px) != 1) {
        exit(1);
    }
    if (scanf("%lf", &py) != 1) {
        exit(1);
    }
    
    in_polygon = (px - DIFF_COORD > py || px < LEFT_B || RIGHT_B < px || py < DOWN_B || UP_B < py);
    

    printf("%d\n", 1 - in_polygon);
    return 0;
}
