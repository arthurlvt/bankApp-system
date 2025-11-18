#ifndef ACCOUNTS_H
#define ACCOUNTS_H 
#include "../include/bank.h" 
void createAccount(Account *account); 
void displayAccountInfo(Account account);
void generateFrenchRIB(char *rib); 
void generateIBAN(char *iban, const char *rib, Nationality nationality); 
void generateBIC(char *bic, Nationality nationality);
int loadAllAccounts(Account accounts[]);
void saveAllAccounts(Account accounts[], int count);
unsigned long hashPassword(const char *password, unsigned long salt);
#endif