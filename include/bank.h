#ifndef BANK_H
#define BANK_H

#include <stdbool.h>
#include <curl/curl.h>

#define MAX_ACCOUNTS         100
#define MAX_PASSWORD_LENGTH  50
#define EXCHANGE_RATES_FILE  "../exchange_rates.txt"

typedef enum {
    USD,
    EUR,
    GBP
} Currency;

typedef struct {
    char name[50];
    int balance;
    int age;
    int gender;
    unsigned long nationality;
    Currency currency;
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

// Account management
int  loadAllAccounts(Account accounts[]);
void saveAllAccounts(Account accounts[], int count);
int  selectAccount(Account accounts[], int *count);
void addMoney(Account accounts[], int count, int currentIndex);
void transferMoney(Account accounts[], int count, int currentIndex);

// Exchange rate management
int   fetchExchangeRates(ExchangeRates *rates, const char *api_key);
void  saveExchangeRates(ExchangeRates rates);
int   loadExchangeRates(ExchangeRates *rates);
void  displayExchangeRates(ExchangeRates rates);

// Currency conversion
double convertCurrency(double amount,
                       Currency fromCurrency,
                       Currency toCurrency,
                       ExchangeRates rates);

#endif // BANK_H
