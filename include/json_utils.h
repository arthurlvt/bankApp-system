#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include "bank.h"
#include <cjson/cJSON.h>

void loadBankFees(const char *filename, Bank *banks, int *count);

#endif // JSON_UTILS_H
