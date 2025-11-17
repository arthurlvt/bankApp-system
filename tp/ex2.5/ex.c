#include <stdio.h>
#include "include.h"

int main() {
    int minNbr, maxNbr;
    printf("Entrez la valeur minimum ainsi que la valeur maximum");
    scanf("%d %d", &minNbr, &maxNbr);
    if (minNbr > maxNbr) {
        if (minNbr < maxNbr) {
            modify(&minNbr, &maxNbr);
            printf("Les nouvelles valeurs sont : min = %d et max = %d\n", minNbr, maxNbr);
        } else {
            printf("Erreur : la valeur minimum doit être inférieure à la valeur maximum.\n");
            printf("Entrez 2 nouvelles valeurs : ");
            scanf("%d %d", &minNbr, &maxNbr);
            modify(&minNbr, &maxNbr);
            printf("Les nouvelles valeurs sont : min = %d et max = %d\n", minNbr, maxNbr);
        }
    } else {
        // les valeurs sont donc inversés
        swap(&minNbr, &maxNbr);
        modify(&minNbr, &maxNbr);
        printf("Erreur! Les valeurs sont inversés! L'ordre est donc inversé!\n Les nouvelles valeurs sont : min = %d et max = %d\n", minNbr, maxNbr);
    }    
}