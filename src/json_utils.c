#include "../include/bank.h"
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void loadBankFees(const char *filename, BankFeesTransfer *fees) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: cannot open file %s\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(file_size + 1);
    if (!buffer) {
        fclose(file);
        printf("Error: cannot allocate memory\n");
        return;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    free(buffer);

    if (!json) {
        printf("Error: cannot parse JSON\n");
        return;
    }

    cJSON *instant_transfer_fee = cJSON_GetObjectItemCaseSensitive(json, "instant_transfer_fee");
    cJSON *standard_transfer_fee = cJSON_GetObjectItemCaseSensitive(json, "standard_transfer_fee");
    cJSON *exchange_rate_margin = cJSON_GetObjectItemCaseSensitive(json, "exchange_rate_margin");

    if (cJSON_IsNumber(instant_transfer_fee) && cJSON_IsNumber(standard_transfer_fee) && cJSON_IsNumber(exchange_rate_margin)) {
        fees->instant_transfer_fee = instant_transfer_fee->valuedouble;
        fees->standard_transfer_fee = standard_transfer_fee->valuedouble;
        fees->exchange_rate_margin = exchange_rate_margin->valuedouble;
    } else {
        printf("Error: invalid fees data\n");
    }

    cJSON_Delete(json);
}