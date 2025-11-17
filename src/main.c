#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "../include/bank.h"
#include "../include/utils.h"

int main() {
    Account accounts[MAX_ACCOUNTS];
    ExchangeRates rates;
    int count = loadAllAccounts(accounts);
    bool adminFound = false;

    // Vérifie si le compte admin existe
    for (int i = 0; i < count; i++) {
        if (strcmp(accounts[i].name, "admin") == 0) {
            adminFound = true;
            break;
        }
    }

    // Making sure an admin account exists (creates one if not)
    if (!adminFound) {
        printf("Creating default admin account...\n");
        strcpy(accounts[count].name, "admin");
        accounts[count].age = 30;
        accounts[count].gender = 0; // assuming 0 = male, 1 = female
        accounts[count].nationality = 0; // assuming 0 = unknown, 1 = French, 2 = American, 3 = British
        accounts[count].balance = 100000; // banque initiale
        accounts[count].currency = EUR;
        accounts[count].salt = (unsigned long)time(NULL);
        accounts[count].passwordHash = hashPassword("admin123", accounts[count].salt);
        count++;
        saveAllAccounts(accounts, count);
        printf("✅ Admin account created (username: admin / password: admin123)\n");
    }

    printf("\n=== Welcome to the International ATM Machine! ===\n\n");
    int index = selectAccount(accounts, &count);
    if (index < 0) {
        printf("Exiting...\n");
        return 0;
    }

    Account *user = &accounts[index];

    // Conversion devise en chaîne
    const char *currencyStr;
    switch (user->currency) {
        case USD: currencyStr = "USD"; break;
        case EUR: currencyStr = "EUR"; break;
        case GBP: currencyStr = "GBP"; break;
        default: currencyStr = "UNKNOWN"; break;
    }

    char input[32];
    int choice = 0;

    do {
        printf("\n===== ATM MENU for %s =====\n", user->name);
        printf("1. Add Money (admin only)\n");
        printf("2. Take cash\n");
        printf("3. Display Balance\n");
        printf("4. Transfer Money\n");
        printf("5. Check Exchange Rates\n");
        printf("6. Refresh Exchange Rates from API\n");
        printf("7. Convert Currency\n");
        printf("8. Save and Quit\n");

        // Lecture sécurisée
        scanf("%31s", input);
        char *endptr;
        choice = strtol(input, &endptr, 10);

        if (*endptr != '\0') {
            printf("❌ Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1: {
                addMoney(accounts, count, index);
                saveAllAccounts(accounts, count);
                break;
            }

            case 2: {
                int amount;
                printf("How much would you like to extract? ");
                if (scanf("%d", &amount) != 1 || amount <= 0) {
                    printf("❌ Invalid amount.\n");
                    while (getchar() != '\n'); // vide le buffer
                    break;
                }
                if (amount > user->balance)
                    printf("❌ Insufficient funds.\n");
                else {
                    user->balance -= amount;
                    printf("✅ New balance: %d %s\n", user->balance, currencyStr);
                    saveAllAccounts(accounts, count);
                }
                break;
            }

            case 3: {
                printf("💰 Current balance: %d %s\n", user->balance, currencyStr);
                break;
            }

            case 4: {
                transferMoney(accounts, count, index);
                saveAllAccounts(accounts, count);
                break;
            }

            case 5: {
                printf("Fetching exchange rates from API...\n");
                if (fetchExchangeRates(&rates, "d5f0a7884008ab629efea30c") == 0) {
                    printf("✅ Exchange rates fetched successfully and saved to file.\n");
                } else {
                    printf("⚠️ Failed to fetch exchange rates from API. Trying to load saved rates...\n");
                    if (loadExchangeRates(&rates) == 0) {
                        printf("✅ Loaded exchange rates from file.\n");
                    } else {
                        printf("❌ No saved exchange rates found. Using default values.\n");
                        rates.eur_to_usd = 1.08;
                        rates.eur_to_gbp = 0.85;
                        rates.usd_to_eur = 0.93;
                        rates.usd_to_gbp = 0.79;
                        rates.gbp_to_eur = 1.18;
                        rates.gbp_to_usd = 1.27;
                    }
                }
                displayExchangeRates(rates);
                break;
            }

            case 6: {
                printf("Fetching latest exchange rates from API...\n");
                if (fetchExchangeRates(&rates, "d5f0a7884008ab629efea30c") == 0)
                    printf("✅ Exchange rates updated successfully.\n");
                else
                    printf("⚠️ Failed to fetch new exchange rates.\n");
                break;
            }

            case 7: {
                int fromCurrency, toCurrency;
                double amount;
                printf("Enter the amount to convert: ");
                if (scanf("%lf", &amount) != 1 || amount <= 0) {
                    printf("❌ Invalid amount.\n");
                    while (getchar() != '\n');
                    break;
                }
                printf("From currency (0=USD, 1=EUR, 2=GBP): ");
                if (scanf("%d", &fromCurrency) != 1 || fromCurrency < 0 || fromCurrency > 2) {
                    printf("❌ Invalid input.\n");
                    while (getchar() != '\n');
                    break;
                }
                printf("To currency (0=USD, 1=EUR, 2=GBP): ");
                if (scanf("%d", &toCurrency) != 1 || toCurrency < 0 || toCurrency > 2) {
                    printf("❌ Invalid input.\n");
                    while (getchar() != '\n');
                    break;
                }
                double convertedAmount = convertCurrency(amount, fromCurrency, toCurrency, rates);
                printf("💱 Converted amount: %.2f\n", convertedAmount);
                break;
            }

            case 8: {
                printf("💾 Saving and exiting...\n");
                saveAllAccounts(accounts, count);
                break;
            }

            default:
                printf("❌ Invalid choice. Please try again.\n");
        }

    } while (choice != 8);

    return 0;
}
