#include <string.h>
#include <stdbool.h>
#include "../include/utils.h"

// ============================================================================
//  Simple password hashing function using a salted variant of djb2
// ============================================================================
unsigned long hashPassword(const char *password, unsigned long salt) {
    unsigned long hash = salt;
    int c;

    while ((c = *password++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }

    return hash;
}

// ============================================================================
//  Verify that a given password matches the stored hash
// ============================================================================
bool verifyPassword(const char *inputPassword, unsigned long storedHash, unsigned long salt) {
    unsigned long inputHash = hashPassword(inputPassword, salt);
    return inputHash == storedHash;
}
