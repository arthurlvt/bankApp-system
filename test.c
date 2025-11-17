#include <stdio.h>

int main () 
{
    int array [6] = { 3, 15, 2, 28};
    array[4] = 60;
    printf("Quelle valeur entière souhaitez entrer? ");
    scanf("%d", &array[5]);
    for (int i  = 0 ;  i < 6 ; i=i+1)
    {
        if (array[i]%2 == 0 || array[i] == 0) {
            if (array[i] == array[i+1]) {
                printf("%d", array[i]);
            }
            else {
                printf("%d ", array[i]);
            }
        }
    }
}