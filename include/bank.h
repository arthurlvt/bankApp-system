#ifndef BANK_H
#define BANK_H

#include <stdbool.h>
#include <curl/curl.h>

#define MAX_ACCOUNTS 100
#define MAX_PASSWORD_LENGTH 50

typedef struct {
    char name[50];
    int balance;
    int currency;
    unsigned long passwordHash;
    unsigned long salt;
} Account;

typedef struct {
    double eur_to_usd;
    double eur_to_gbp;
    double usd_to_eur;
    double usd_to_gbp;
    double gbp_to_eur;
    double gbp_to_usd;
} ExchangeRates;

// Prototypes of functions
int loadAllAccounts(Account accounts[]);
void saveAllAccounts(Account accounts[], int count);
int selectAccount(Account accounts[], int *count);
void transferMoney(Account accounts[], int count, int currentIndex);
int fetchExchangeRates(ExchangeRates *rates, const char *api_key);
void displayExchangeRates(ExchangeRates rates);
double convertCurrency(double amount, int fromCurrency, int toCurrency, ExchangeRates rates);

#endif
