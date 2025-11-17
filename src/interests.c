#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/bank.h"

#define INTEREST_RATES_FILE "interest_rates.dat"
// Saves interest rates to a file
void save_interest_rates(double *rates, int count) {
    FILE *file = fopen(INTEREST_RATES_FILE, "wb");
    if (!file) {
        perror("Failed to open interest rates file");
        return;
    }
    fprintf(file, "Frais - Virements Instantannés\n");
    fwrite(rates, sizeof(double), count, file);
    fclose(file);
}
// Loads interest rates from a file
int load_interest_rates(double *rates, int count) {
    FILE *file = fopen(INTEREST_RATES_FILE, "rb");
    if (!file) {
        perror("Failed to open interest rates file");
        return -1;
    }
    size_t read_count = fread(rates, sizeof(double), count, file);
    fclose(file);
    return read_count == count ? 0 : -1;
};

int get_interest_rate(double *rates, int count, const char *account_type) {
    
}