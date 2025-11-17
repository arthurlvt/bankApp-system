#include <stdio.h>
#include "include.h"
void adjustSalary(double *sal1, double *sal2, double *sal3, double *bonus) {
    *sal1 = *sal1 + *bonus/3;
    *sal2 = *sal2 + *bonus/3;
    *sal3 = *sal3 + *bonus/3;
}