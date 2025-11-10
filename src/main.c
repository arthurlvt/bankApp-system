#include <stdio.h>
#include "../include/bank.h"
#include "../include/utils.h"
#include "../include/accounts.h"

int main() {
    Account accounts[MAX_ACCOUNTS];
    ExchangeRates rates;
    int count = loadAllAccounts(accounts);

    // Replace with your actual API key
    if (fetchExchangeRates(&rates, "YOUR API KEY") != 0) {
        printf("Failed to fetch exchange rates. Using default values.\n");
        // Use default values in case of failure
        rates.eur_to_usd = 1.08;
        rates.eur_to_gbp = 0.85;
        rates.usd_to_eur = 0.93;
        rates.usd_to_gbp = 0.79;
        rates.gbp_to_eur = 1.18;
        rates.gbp_to_usd = 1.27;
    }

    printf("=== Welcome to the International ATM Machine! ===\n\n");
    int index = selectAccount(accounts, &count);
    if (index < 0) {
        printf("Exiting...\n");
        return 0;
    }
    Account *user = &accounts[index];
    const char *currencyStr = (user->currency == 1) ? "USD" :
                              (user->currency == 2) ? "EUR" : "GBP";
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
                printf("From currency (1=USD, 2=EUR, 3=GBP): ");
                scanf("%d", &fromCurrency);
                printf("To currency (1=USD, 2=EUR, 3=GBP): ");
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