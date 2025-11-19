#include <stdbool.h>
#ifndef UTILS_H
#define UTILS_H

unsigned long hashPassword(const char *password, unsigned long salt);
bool verifyPassword(const char *inputPassword, unsigned long storedHash, unsigned long salt);

#endif