#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/bank.h"
#include "../include/accounts.h"
#include "../include/utils.h"
#include "../include/logs.h"

void displayMainMenu() {
    printf("\n===== Main Menu =====\n");
    printf("1. Create Account\n");
    printf("2. Log In\n");
    printf("3. Exit\n");
    printf("> ");
}
void displayAccountMenu(const Account *account) {
    printf("\n===== Account Menu for %s =====\n", account->name);
    printf("1. View Account Info\n");
    printf("2. Transfer Money\n");
    printf("3. Add Money (Admin Only)\n");
    printf("4. Log Out\n");
    printf("> ");
}

int main() {
    Account accounts[MAX_ACCOUNTS];
    int accountCount = loadAllAccounts(accounts);

    int choice;
    int loggedIn = 0;
    int currentAccountIndex = -1;

    initLogs();
    writeLog("ATM started");

    do {
        if (!loggedIn) {
            displayMainMenu();
            scanf("%d", &choice);

            switch (choice) {
                case 1: {
                    if (accountCount >= MAX_ACCOUNTS) {
                        printf("Maximum number of accounts reached.\n");
                        break;
                    }
                    writeLog("User started 'create account' option in main menu");
                    Account newAccount;
                    createAccount(&newAccount);
                    accounts[accountCount] = newAccount;
                    accountCount++;
                    saveAllAccounts(accounts, accountCount);
                    printf("Account created successfully!\n");
                    break;
                }
                case 2: {
                    char name[50];
                    char password[MAX_PASSWORD_LENGTH];
                    printf("Enter account name: ");
                    scanf("%49s", name);
                    printf("Enter password: ");
                    scanf("%49s", password);


                    int found = 0;
                    for (int i = 0; i < accountCount; i++) {
                        if (strcmp(accounts[i].name, name) == 0) {
                            unsigned long hashedPassword = hashPassword(password, accounts[i].salt);
                            if (accounts[i].passwordHash == hashedPassword) {
                                currentAccountIndex = i;
                                loggedIn = 1;
                                found = 1;
                                printf("Login successful!\n");
                                break;
                            }
                        }
                    }

                    if (!found) {
                        printf("Invalid account name or password.\n");
                    }
                    writeLog("User logged in");
                    break;
                }
                case 3:
                    printf("Exiting...\n");
                    break;
                default:
                    printf("Invalid choice.\n");
            }
        } else {
            displayAccountMenu(&accounts[currentAccountIndex]);
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    displayAccountInfo(accounts[currentAccountIndex]);
                    writeLog("User viewed account info");
                    break;
                case 2: {
                    transferMoney(accounts, accountCount, currentAccountIndex);
                    saveAllAccounts(accounts, accountCount);
                    writeLog("User transferred money");
                    break;
                }
                case 3:
                    if (strcmp(accounts[currentAccountIndex].name, "admin") != 0) {
                        printf("❌ Only the admin account can add money.\n");
                        writeLog("User attempted to add money without admin privileges");
                        break;
                    }
                    int amount;
                    printf("Enter amount to add: ");
                    scanf("%d", &amount);
                    accounts[currentAccountIndex].balance += amount;
                    printf("✅ Added %d to %s's balance. New balance: %d\n", amount, accounts[currentAccountIndex].name, accounts[currentAccountIndex].balance);
                    saveAllAccounts(accounts, accountCount);
                    writeLog("Admin added money to account");
                    break;
                case 4:
                    loggedIn = 0;
                    currentAccountIndex = -1;
                    printf("Logged out successfully.\n");
                    writeLog("User logged out");
                    break;
                default:
                    printf("Invalid choice.\n");
            }
        }
    } while (choice != 3 || loggedIn);

    saveAllAccounts(accounts, accountCount);
    return 0;
}
