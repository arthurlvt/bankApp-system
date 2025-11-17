#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bank.h"

void loadBankFees(const char *filename, BankFeesTransfer *fees) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: cannot open file %s\n", filename);
        fees->instant_transfer_fee = 1.0;
        fees->standard_transfer_fee = 0.5;
        fees->exchange_rate_margin = 1.0;
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(file_size + 1);
    if (!buffer) {
        fclose(file);
        printf("Error: cannot allocate memory\n");
        fees->instant_transfer_fee = 1.0;
        fees->standard_transfer_fee = 0.5;
        fees->exchange_rate_margin = 1.0;
        return;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    free(buffer);

    if (!json) {
        printf("Error: cannot parse JSON\n");
        fees->instant_transfer_fee = 1.0;
        fees->standard_transfer_fee = 0.5;
        fees->exchange_rate_margin = 1.0;
        return;
    }

    cJSON *instant_fee = cJSON_GetObjectItemCaseSensitive(json, "instant_transfer_fee");
    cJSON *standard_fee = cJSON_GetObjectItemCaseSensitive(json, "standard_transfer_fee");
    cJSON *exchange_margin = cJSON_GetObjectItemCaseSensitive(json, "exchange_rate_margin");

    if (cJSON_IsNumber(instant_fee)) {
        fees->instant_transfer_fee = instant_fee->valuedouble;
    } else {
        fees->instant_transfer_fee = 1.0;
    }

    if (cJSON_IsNumber(standard_fee)) {
        fees->standard_transfer_fee = standard_fee->valuedouble;
    } else {
        fees->standard_transfer_fee = 0.5;
    }

    if (cJSON_IsNumber(exchange_margin)) {
        fees->exchange_rate_margin = exchange_margin->valuedouble;
    } else {
        fees->exchange_rate_margin = 1.0;
    }

    cJSON_Delete(json);
}