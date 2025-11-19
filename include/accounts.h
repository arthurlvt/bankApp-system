#ifndef ACCOUNTS_H
#define ACCOUNTS_H 
#include "../include/bank.h" 

#define ACCOUNTS_DATA_FILE "data/accounts.dat"
#define MAX_ACCOUNTS 100

int loadAllAccounts(Account accounts[]);
void createAdminAccount(Account accounts[], int *count);
void saveAllAccounts(Account accounts[], int count);
void createAccount(Account *account);
void displayAccountInfo(Account account);
void generateFrenchRIB(char *rib);
void generateIBAN(char *iban, const char *rib, Nationality nationality);
void generateBIC(char *bic, Nationality nationality);

#endif