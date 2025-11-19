#ifndef BANK_H
#define BANK_H

#include <stdbool.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#define MAX_ACCOUNTS         100
#define MAX_PASSWORD_LENGTH  50
#define MAX_RIB_LENGTH 24
#define MAX_IBAN_LENGTH 35
#define MAX_BIC_LENGTH 12
#define EXCHANGE_RATES_FILE  "data/exchange_rates.json"


// Account structure
typedef enum {
    USD,
    EUR,
    GBP,
    AUD,
    DZD,
} Currency;

typedef enum {
    UNKNOWN,
    FRENCH,
    AMERICAN,
    BRITISH,
    GERMAN,
    ITALIAN,
    SPANISH,
    IRISH,
    AUSTRALIAN,
    ALGERIAN,
} Nationality;

typedef enum {
    MALE,
    FEMALE,
    DOG,
    CAT,
    NON_BINARY,
    RATHER_NOT_SAY,
    MTF,
    FTM,
    HELICOPTER,
    OTHER
} Gender;

typedef struct {
    char name[50];
    int age;
    int gender;
    Nationality nationality;
    Currency currency;
    int balance;
    char rib[MAX_RIB_LENGTH];
    char iban[MAX_IBAN_LENGTH];
    char bic[MAX_BIC_LENGTH];
    unsigned long passwordHash;
    unsigned long salt;
} Account;

// Exchange rates structure
typedef struct {
    double eur_to_usd;
    double eur_to_gbp;
    double usd_to_eur;
    double usd_to_gbp;
    double gbp_to_eur;
    double gbp_to_usd;
} ExchangeRates;


// Bank Fees Transfer Structure
typedef struct {
    double instant_transfer_fee;
    double standard_transfer_fee;
    double exchange_rate_margin;
} BankFeesTransfer;

typedef struct {
    char name[50];
    BankFeesTransfer fees;
} Bank;FeesRecord;

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
double convertCurrency(double amount, Currency fromCurrency, Currency toCurrency, ExchangeRates rates);
// Bank fees management
void loadBankFees(const char *filename, BankFeesTransfer *fees);

#endif // BANK_H
