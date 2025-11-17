#include <stdio.h>
#include <string.h>
#include "../include/bank.h"

void addMoney(Account accounts[], int count, int currentIndex) {
    (void)count;
    Account *account = &accounts[currentIndex];
    if (strcmp(account->name, "admin") != 0) {
        printf("❌ Only the admin account can add money.\n");
        return;
    }
    int amount;
    printf("Enter amount to add: ");
    scanf("%d", &amount);

    account->balance += amount;
    printf("✅ Added %d to admin balance. New balance: %d\n", amount, account->balance);
}

void transferMoney(Account accounts[], int count, int currentIndex) {
    printf("\n=== Transfer Money ===\n");
    printf("Select the destination account:\n");

    // Display available destination accounts
    for (int i = 0; i < count; i++) {
        if (i != currentIndex) {
            printf("%d. %s\n", i + 1, accounts[i].name);
        }
    }

    printf("> ");
    int destChoice;
    scanf("%d", &destChoice);

    // Validate destination choice
    if (destChoice < 1 || destChoice > count || destChoice == currentIndex + 1) {
        printf("Invalid choice.\n");
        return;
    }

    int destIndex = destChoice - 1;

    // Ask for transfer amount
    printf("Enter the amount to transfer: ");
    int amount;
    scanf("%d", &amount);

    // Validate amount
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
