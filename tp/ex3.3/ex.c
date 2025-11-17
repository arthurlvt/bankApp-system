#include <stdio.h>
#include <stdbool.h>

int main() {
    int a, b;
    bool val1, val2;
    bool resultOR = false, resultAND = false, resultXOR = false;
    int o1, o2, o3;

    printf("Enter first boolean value (0 or 1): ");
    scanf("%d", &a);
    printf("Enter second boolean value (0 or 1): ");
    scanf("%d", &b);

    val1 = (bool)a;
    val2 = (bool)b;

    logicOperation(val1, val2, o1, o2, o3, &resultOR, &resultAND, &resultXOR);

    if (o1) printf("Result of OR operation: %d\n", resultOR);
    if (o2) printf("Result of AND operation: %d\n", resultAND);
    if (o3) printf("Result of XOR operation: %d\n", resultXOR);

    return 0;
}
