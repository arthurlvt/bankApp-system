// Ex 2.4 Salary adjustment with pointers
#include <stdio.h>
#include "include.h"

int main() {
    double salary1, salary2, salary3, bonus;
    printf("Enter the salaries of three employees:\n");
    scanf("%lf %lf %lf", &salary1, &salary2, &salary3);
    printf("Enter the total bonus amount to be distributed:\n");
    scanf("%lf", &bonus);
    adjustSalary(&salary1, &salary2, &salary3, &bonus);
    printf("Enter the total bonus amount to be distributed:\n");
    scanf("%lf", &bonus);
    adjustSalary(&salary1, &salary2, &salary3, &bonus);

    printf("Salaries after adjustment:\n");
    printf("Person 1: %.2f\n", salary1);
    printf("Person 2: %.2f\n", salary2);
    printf("Person 3: %.2f\n", salary3);
}

