#include "unity.h"
#include "../include/utils.h"

void setUp(void) {
    // Cette fonction est appelée avant chaque test
}

void tearDown(void) {
    // Cette fonction est appelée après chaque test
}

void test_hashPassword(void) {
    unsigned long salt = 12345;
    unsigned long hash = hashPassword("test", salt);
    TEST_ASSERT_NOT_EQUAL(0, hash);
}

void test_verifyPassword(void) {
    unsigned long salt = 12345;
    unsigned long hash = hashPassword("test", salt);
    TEST_ASSERT_TRUE(verifyPassword("test", hash, salt));
    TEST_ASSERT_FALSE(verifyPassword("wrong", hash, salt));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_hashPassword);
    RUN_TEST(test_verifyPassword);
    return UNITY_END();
}
