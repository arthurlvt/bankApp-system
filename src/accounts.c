#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/bank.h"
#include "../include/utils.h"
#include "../include/accounts.h"

int loadAllAccounts(Account accounts[]) {
    FILE *file = fopen("../data/accounts.txt", "r");
    if (!file)
        return 0;

    int count = 0;
    while (
        count < MAX_ACCOUNTS &&
        fscanf(file, "%49s %d %d %lu %lu",
               accounts[count].name,
               &accounts[count].age,
               &accounts[count].gender,
               &accounts[count].nationality,
               &accounts[count].balance,
               &accounts[count].currency,
               &accounts[count].passwordHash,
               &accounts[count].salt) == 8
    ) {
        count++;
    }
    fclose(file);
    return count;
}

void saveAllAccounts(Account accounts[], int count) {
    FILE *file = fopen("../data/accounts.txt", "w");
    if (!file) {
        printf("Error saving accounts!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d %d %lu %lu\n",
                accounts[i].name,
                accounts[i].balance,
                accounts[i].age,
                accounts[i].gender,
                accounts[i].nationality,
                accounts[i].currency,
                accounts[i].passwordHash,
                accounts[i].salt);
    }

    fclose(file);
}

void createAccount(Account *account) {
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter new account name: ");
    scanf("%49s", account->name);

    printf("Enter age: ");
    scanf("%d", &account->age);

    printf("Enter gender (0=male, 1=female): ");
    scanf("%d", &account->gender);

    printf("Enter nationality (0=unknown), 1=French, 2=American, 3=British: ");
    scanf("%lu", &account->nationality);

    // Gives default balance of 0
    account->balance = 0;

    /*
    printf("Select currency (1=USD, 2=EUR, 3=GBP): ");
    scanf("%d", &account->currency);
    */

    printf("Enter password: ");
    scanf("%49s", password);

    account->salt = (unsigned long)time(NULL); // Use current time as salt
    account->passwordHash = hashPassword(password, account->salt);
}

int selectAccount(Account accounts[], int *count) {
    int choice;

    do {
        printf("=== Account Management ===\n");

        for (int i = 0; i < *count; i++) {
            printf("%d. %s\n", i + 1, accounts[i].name);
        }

        printf("%d. Create new account\n", *count + 1);
        printf("%d. Delete an account\n", *count + 2);
        printf("%d. Exit\n", *count + 3);
        printf("> ");
        scanf("%d", &choice);

        if (choice >= 1 && choice <= *count) {
            char inputPassword[MAX_PASSWORD_LENGTH];
            printf("Enter password: ");
            scanf("%49s", inputPassword);

            if (!verifyPassword(inputPassword,
                                accounts[choice - 1].passwordHash,
                                accounts[choice - 1].salt)) {
                printf("Incorrect password. Access denied.\n");
                continue;
            }

            printf("✅ Welcome back, %s!\n", accounts[choice - 1].name);
            return choice - 1;
        }

        else if (choice == *count + 1) {
            if (*count >= MAX_ACCOUNTS) {
                printf("Error: maximum accounts reached!\n");
                return -1;
            }

            createAccount(&accounts[*count]);
            (*count)++;
            printf("✅ Account created!\n");
            return *count - 1;
        }

        else if (choice == *count + 2) {
            if (*count <= 0) {
                printf("No accounts to delete.\n");
                continue;
            }

            printf("=== Delete an Account ===\n");
            for (int i = 0; i < *count; i++) {
                printf("%d. %s\n", i + 1, accounts[i].name);
            }

            printf("Select the account to delete (1-%d, 0 to cancel): ", *count);
            int deleteChoice;
            scanf("%d", &deleteChoice);

            if (deleteChoice < 1 || deleteChoice > *count) {
                printf("Invalid choice or deletion cancelled.\n");
                continue;
            }

            char inputPassword[MAX_PASSWORD_LENGTH];
            printf("Enter password: ");
            scanf("%49s", inputPassword);

            if (!verifyPassword(inputPassword,
                                accounts[deleteChoice - 1].passwordHash,
                                accounts[deleteChoice - 1].salt)) {
                printf("Incorrect password. Deletion cancelled.\n");
                continue;
            }

            // ✅ Suppression réelle du compte
            for (int i = deleteChoice - 1; i < *count - 1; i++) {
                accounts[i] = accounts[i + 1];
            }
            (*count)--;

            printf("Account deleted successfully.\n");
            continue;
        }

        else if (choice == *count + 3) {
            printf("Exiting...\n");
            return -1;
        }

        else {
            printf("Invalid choice.\n");
        }

    } while (1);
}
