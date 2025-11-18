#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        fscanf(file, "%49s %d %d %u %d %u %lu %lu %23s %34s %11s",
               accounts[count].name,
               &accounts[count].age,
               &accounts[count].gender,
               (unsigned int*)&accounts[count].nationality,
               &accounts[count].balance,
               (unsigned int*)&accounts[count].currency,
               &accounts[count].passwordHash,
               &accounts[count].salt,
               accounts[count].rib,
               accounts[count].iban,
               accounts[count].bic) == 11
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
        fprintf(file, "%s %d %d %u %d %u %lu %lu %s %s %s\n",
                accounts[i].name,
                accounts[i].age,
                accounts[i].gender,
                (unsigned int)accounts[i].nationality,
                accounts[i].balance,
                (unsigned int)accounts[i].currency,
                accounts[i].passwordHash,
                accounts[i].salt,
                accounts[i].rib,
                accounts[i].iban,
                accounts[i].bic);
    }
    fclose(file);
}

void generateFrenchRIB(char *rib) {
    const char *bankCode = "30006";
    const char *counterCode = "00001";
    const char *accountNumber = "12345678901";
    const char *ribKey = "89";
    snprintf(rib, MAX_RIB_LENGTH, "%s%s%s%s", bankCode, counterCode, accountNumber, ribKey);
}

void generateIBAN(char *iban, const char *rib, Nationality nationality) {
    if (nationality == FRENCH && strlen(rib) > 0) {
        const char *countryCode = "FR";
        const char *checkDigits = "76";
        snprintf(iban, MAX_IBAN_LENGTH, "%s%s%s", countryCode, checkDigits, rib);
    } else if (nationality == AMERICAN) {
        snprintf(iban, MAX_IBAN_LENGTH, "US64SVBKUS6S3300958879");
    } else if (nationality == BRITISH) {
        snprintf(iban, MAX_IBAN_LENGTH, "GB29NWBK60161331926819");
    }
}

void generateBIC(char *bic, Nationality nationality) {
    if (nationality == FRENCH) {
        snprintf(bic, MAX_BIC_LENGTH, "SOGEFRPP");
    } else if (nationality == AMERICAN) {
        snprintf(bic, MAX_BIC_LENGTH, "BOFAUS3N");
    } else if (nationality == BRITISH) {
        snprintf(bic, MAX_BIC_LENGTH, "NWBKGB2L");
    }
}

void createAccount(Account *account) {
    char password[MAX_PASSWORD_LENGTH];
    printf("Enter new account name: ");
    scanf("%49s", account->name);
    printf("Enter age: ");
    scanf("%d", &account->age);
    printf("Enter gender (0=male, 1=female): ");
    scanf("%d", &account->gender);
    printf("Enter nationality (0=unknown, 1=French, 2=American, 3=British): ");
    int nationalityChoice;
    scanf("%d", &nationalityChoice);
    account->nationality = (Nationality)nationalityChoice;

    // Set default balance of 0
    account->balance = 0;

    // Set currency based on nationality
    if (account->nationality == FRENCH) {
        account->currency = EUR;
    } else if (account->nationality == AMERICAN) {
        account->currency = USD;
    } else if (account->nationality == BRITISH) {
        account->currency = GBP;
    } else {
        account->currency = USD; // Default currency
    }

    printf("Enter password: ");
    scanf("%49s", password);
    account->salt = (unsigned long)time(NULL); // Use current time as salt
    account->passwordHash = hashPassword(password, account->salt);

    // Generate bank details based on nationality
    if (account->nationality == FRENCH) {
        generateFrenchRIB(account->rib);
        generateIBAN(account->iban, account->rib, account->nationality);
        generateBIC(account->bic, account->nationality);
    } else if (account->nationality == AMERICAN) {
        strcpy(account->rib, ""); // RIB is not applicable for American accounts
        generateIBAN(account->iban, account->rib, account->nationality);
        generateBIC(account->bic, account->nationality);
    } else if (account->nationality == BRITISH) {
        strcpy(account->rib, ""); // RIB is not applicable for British accounts
        generateIBAN(account->iban, account->rib, account->nationality);
        generateBIC(account->bic, account->nationality);
    } else {
        strcpy(account->rib, "");
        strcpy(account->iban, "");
        strcpy(account->bic, "");
    }
}

void displayAccountInfo(Account account) {
    printf("Account Name: %s\n", account.name);
    printf("Age: %d\n", account.age);
    printf("Gender: %s\n", account.gender == 0 ? "Male" : "Female");
    printf("Nationality: ");
    switch (account.nationality) {
        case FRENCH:
            printf("French\n");
            break;
        case AMERICAN:
            printf("American\n");
            break;
        case BRITISH:
            printf("British\n");
            break;
        default:
            printf("Unknown\n");
    }
    printf("Currency: ");
    switch (account.currency) {
        case USD:
            printf("USD\n");
            break;
        case EUR:
            printf("EUR\n");
            break;
        case GBP:
            printf("GBP\n");
            break;
    }
    printf("Balance: %d\n", account.balance);
    if (strlen(account.rib) > 0) {
        printf("RIB: %s\n", account.rib);
    }
    if (strlen(account.iban) > 0) {
        printf("IBAN: %s\n", account.iban);
    }
    if (strlen(account.bic) > 0) {
        printf("BIC: %s\n", account.bic);
    }
}
