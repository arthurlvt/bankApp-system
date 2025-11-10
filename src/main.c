#include <stdio.h>
#include <stdlib.h>
#include "bank.h"
#include "utils.h"

int main() {
    Account accounts[MAX_ACCOUNTS];
    ExchangeRates rates;
    int count = loadAllAccounts(accounts);

    // Charger les taux de change depuis le fichier
    if (loadExchangeRates(&rates) != 0) {
        printf("No saved exchange rates found. Fetching from API...\n");
        // Remplace "TON_API_KEY" par ta clé API réelle
        if (fetchExchangeRates(&rates, "d5f0a7884008ab629efea30c") != 0) {
            printf("Failed to fetch exchange rates. Using default values.\n");
            // Utilise des valeurs par défaut en cas d'échec
            rates.eur_to_usd = 1.08;
            rates.eur_to_gbp = 0.85;
            rates.usd_to_eur = 0.93;
            rates.usd_to_gbp = 0.79;
            rates.gbp_to_eur = 1.18;
            rates.gbp_to_usd = 1.27;
            saveExchangeRates(rates); // Sauvegarde les valeurs par défaut
        }
    } else {
        printf("Loaded exchange rates from file.\n");
    }

    printf("=== Welcome to the International ATM Machine! ===\n\n");
    int index = selectAccount(accounts, &count);
    if (index < 0) {
        printf("Exiting...\n");
        return 0;
    }
    Account *user = &accounts[index];
    const char *currencyStr = (user->currency == USD) ? "USD" :
                              (user->currency == EUR) ? "EUR" : "GBP";
    int choice;
    do {
        printf("\n===== ATM MENU for %s =====\n", user->name);
        printf("1. Add Money\n");
        printf("2. Extract Money\n");
        printf("3. Display Balance\n");
        printf("4. Transfer Money\n");
        printf("5. Check Exchange Rates\n");
        printf("6. Convert Currency\n");
        printf("7. Save and Quit\n> ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                int amount;
                printf("How much would you like to add? ");
                scanf("%d", &amount);
                user->balance += amount;
                printf("New balance: %d %s\n", user->balance, currencyStr);
                break;
            }
            case 2: {
                int amount;
                printf("How much would you like to extract? ");
                scanf("%d", &amount);
                if (amount > user->balance)
                    printf("Insufficient funds.\n");
                else {
                    user->balance -= amount;
                    printf("New balance: %d %s\n", user->balance, currencyStr);
                }
                break;
            }
            case 3:
                printf("Current balance: %d %s\n", user->balance, currencyStr);
                break;
            case 4:
                transferMoney(accounts, count, index);
                break;
            case 5:
                displayExchangeRates(rates);
                break;
            case 6: {
                int fromCurrency, toCurrency;
                double amount;
                printf("Enter the amount to convert: ");
                scanf("%lf", &amount);
                printf("From currency (0=USD, 1=EUR, 2=GBP): ");
                scanf("%d", &fromCurrency);
                printf("To currency (0=USD, 1=EUR, 2=GBP): ");
                scanf("%d", &toCurrency);
                double convertedAmount = convertCurrency(amount, fromCurrency, toCurrency, rates);
                printf("Converted amount: %.2f\n", convertedAmount);
                break;
            }
            case 7:
                printf("Saving and exiting...\n");
                saveAllAccounts(accounts, count);
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 7);
    return 0;
}
