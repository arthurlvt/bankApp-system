#include <stdio.h>
#include "../include/bank.h"

// Transfer money between accounts
void transferMoney(Account accounts[], int count, int currentIndex) {
    printf("\n=== Transfer Money ===\n");
    printf("Select the destination account:\n");
    for (int i = 0; i < count; i++) {
        if (i != currentIndex) {
            printf("%d. %s\n", i + 1, accounts[i].name);
        }
    }
    printf("> ");
    int destChoice;
    scanf("%d", &destChoice);
    if (destChoice < 1 || destChoice > count || destChoice == currentIndex + 1) {
        printf("Invalid choice.\n");
        return;
    }
    int destIndex = destChoice - 1;
    printf("Enter the amount to transfer: ");
    int amount;
    scanf("%d", &amount);
    if (amount <= 0) {
        printf("Invalid amount.\n");
        return;
    }
    if (accounts[currentIndex].balance < amount) {
        printf("Insufficient funds.\n");
        return;
    }
    // Perform the transaction
    accounts[currentIndex].balance -= amount;
    accounts[destIndex].balance += amount;
    printf("✅ Transfer successful!\n");
}
