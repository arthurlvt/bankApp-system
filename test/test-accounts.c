#include "unity.h"
#include "../include/bank.h"

void setUp(void) {
    // Cette fonction est appelée avant chaque test
}

void tearDown(void) {
    // Cette fonction est appelée après chaque test
}

void test_createAccount(void) {
    Account account;
    char password[MAX_PASSWORD_LENGTH] = "test";

    strncpy(account.name, "Test User", 50);
    account.balance = 1000;
    account.currency = EUR;
    account.salt = 12345;
    account.passwordHash = hashPassword(password, account.salt);

    TEST_ASSERT_EQUAL_STRING("Test User", account.name);
    TEST_ASSERT_EQUAL(1000, account.balance);
    TEST_ASSERT_EQUAL(EUR, account.currency);
    TEST_ASSERT_NOT_EQUAL(0, account.passwordHash);
}

void test_loadAndSaveAccounts(void) {
    Account accounts[MAX_ACCOUNTS];
    int count = 0;

    // Créer un compte de test
    strncpy(accounts[count].name, "Test User", 50);
    accounts[count].balance = 1000;
    accounts[count].currency = EUR;
    accounts[count].salt = 12345;
    accounts[count].passwordHash = hashPassword("test", accounts[count].salt);
    count++;

    // Sauvegarder et recharger les comptes
    saveAllAccounts(accounts, count);
    int loadedCount = loadAllAccounts(accounts);

    TEST_ASSERT_EQUAL(count, loadedCount);
    TEST_ASSERT_EQUAL_STRING("Test User", accounts[0].name);
    TEST_ASSERT_EQUAL(1000, accounts[0].balance);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_createAccount);
    RUN_TEST(test_loadAndSaveAccounts);
    return UNITY_END();
}
