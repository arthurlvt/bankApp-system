#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../include/bank.h"
#include "../include/accounts.h"
#include "../include/utils.h"
#define ACCOUNTS_DATA_FILE "data/accounts.dat"

void createAdminAccount(Account accounts[], int *count) {
    // Check if admin account already exists
    for ( int i = 0; i < *count; i++ ) {
        if (strcmp(accounts[i].name, "admin") == 0) {
            return;
        }
    }
    Account adminAccount;
    strcpy(adminAccount.name, "admin");
    adminAccount.age = 30;
    adminAccount.gender = 0;
    adminAccount.nationality = UNKNOWN;
    adminAccount.currency = USD;
    adminAccount.balance = 10000;
    adminAccount.salt = (unsigned long)time(NULL);
    adminAccount.passwordHash = hashPassword("admin", adminAccount.salt);
    strcpy(adminAccount.rib, "");
    strcpy(adminAccount.iban, "");
    strcpy(adminAccount.bic, "");

    accounts[*count] = adminAccount;
    (*count)++;
}

int loadAllAccounts(Account accounts[]) {
    FILE *file = fopen(ACCOUNTS_DATA_FILE, "rb");
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
    createAdminAccount(accounts, &count);
    return count;
}

void saveAllAccounts(Account accounts[], int count) {
    FILE *file = fopen(ACCOUNTS_DATA_FILE, "wb");
    if (!file) {
        printf("Error saving accounts!\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d %d %u %d %u %lu %lu\n",
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
    // Simple RIB generation: 23 digits
    snprintf(rib, MAX_RIB_LENGTH, "30004%05d%011d%02d",
             rand() % 100000, rand() % 100000000000, rand() % 100);
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
    } else if (nationality == GERMAN) {
        snprintf(iban, MAX_IBAN_LENGTH, "DE89370400440532013000");
    } else if (nationality == ITALIAN) {
        snprintf(iban, MAX_IBAN_LENGTH, "IT60X0542811101000000123456");
    } else if (nationality == SPANISH) {
        snprintf(iban, MAX_IBAN_LENGTH, "ES9121000418450200051332");
    } else if (nationality == IRISH) {
        snprintf(iban, MAX_IBAN_LENGTH, "IE29AIBK93115212345678");
    } else if (nationality == AUSTRALIAN) {
        snprintf(iban, MAX_IBAN_LENGTH, "AU690070001234567890123456");
    } else if (nationality == ALGERIAN) {
        snprintf(iban, MAX_IBAN_LENGTH, "DZ400040004000001234567");
    } else {
        snprintf(iban, MAX_IBAN_LENGTH, "UNKNOWN00000000000000000000");
    }
}
void generateBIC(char *bic, Nationality nationality) {
    if (nationality == FRENCH) {
        snprintf(bic, MAX_BIC_LENGTH, "SOGEFRPP");
    } else if (nationality == AMERICAN) {
        snprintf(bic, MAX_BIC_LENGTH, "BOFAUS3N");
    } else if (nationality == BRITISH) {
        snprintf(bic, MAX_BIC_LENGTH, "NWBKGB2L");
    } else if (nationality == GERMAN) {
        snprintf(bic, MAX_BIC_LENGTH, "DEUTDEBB");
    } else if (nationality == ITALIAN) {
        snprintf(bic, MAX_BIC_LENGTH, "UNCRITMM");
    } else if (nationality == SPANISH) {
        snprintf(bic, MAX_BIC_LENGTH, "BBVAESMM");
    } else if (nationality == IRISH) {
        snprintf(bic, MAX_BIC_LENGTH, "AIBKIE2D");
    } else if (nationality == AUSTRALIAN) {
        snprintf(bic, MAX_BIC_LENGTH, "ANZBAU3M");
    } else if (nationality == ALGERIAN) {
        snprintf(bic, MAX_BIC_LENGTH, "BNADDZAL");
    } else {
        snprintf(bic, MAX_BIC_LENGTH, "UNKNOWNXX");
    }
}

void createAccount(Account *account) {
    char password[MAX_PASSWORD_LENGTH];
    printf("Enter new account name: ");
    scanf("%49s", account->name);
    printf("Enter age: ");
    scanf("%d", &account->age);
    printf("Enter gender (0=male, 1=female, 2=dog, 3=cat, 4=non-binary, 5=rather not say, 6=MTF, 7=FTM, 8=helicopter, 9=other): ");
    int genderChoice;
    scanf("%d", &genderChoice);
    account->gender = (Gender)genderChoice;
    printf("Enter nationality (0=unknown, 1=French, 2=American, 3=British, 4=German, 5=Italian, 6=Spanish, 7=Irish, 8=Australian, 9=Algerian): ");
    int nationalityChoice;
    scanf("%d", &nationalityChoice);
    account->nationality = (Nationality)nationalityChoice;

    // Set default balance of 0
    account->balance = 0;

    // Set currency based on nationality
    switch (account->nationality) {
        case FRENCH:
        case GERMAN:
        case ITALIAN:
        case SPANISH:
        case IRISH:
            account->currency = EUR;
            break;
        case AMERICAN:
        case AUSTRALIAN:
            account->currency = USD;
            break;
        case BRITISH:
            account->currency = GBP;
            break;
        case ALGERIAN:
            account->currency = DZD; 
            break;
        default:
            account->currency = USD;
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
    } else {
        strcpy(account->rib, "");
        generateIBAN(account->iban, account->rib, account->nationality);
        generateBIC(account->bic, account->nationality);
    }
}


void displayAccountInfo(Account account) {
    printf("Account Name: %s\n", account.name);
    printf("Age: %d\n", account.age);
    printf("Gender: ");
    switch (account.gender) {
        case MALE:
            printf("Male\n");
            break;
        case FEMALE:
            printf("Female\n");
            break;
        case DOG:
            printf("Dog\n");
            break;
        case CAT:
            printf("Cat\n");
            break;
        case NON_BINARY:
            printf("Non-binary\n");
            break;
        case RATHER_NOT_SAY:
            printf("Rather not say\n");
            break;
        case MTF:
            printf("MTF\n");
            break;
        case FTM:
            printf("FTM\n");
            break;
        case HELICOPTER:
            printf("Helicopter\n");
            break;
        case OTHER:
            printf("Other\n");
            break;
    }
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
        case GERMAN:
            printf("German\n");
            break;
        case ITALIAN:
            printf("Italian\n");
            break;
        case SPANISH:
            printf("Spanish\n");
            break;
        case IRISH:
            printf("Irish\n");
            break;
        case AUSTRALIAN:
            printf("Australian\n");
            break;
        case ALGERIAN:
            printf("Algerian\n");
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
