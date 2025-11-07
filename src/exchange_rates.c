#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "../include/bank.h"

struct MemoryStruct {
    char *memory;
    size_t size;
};

// Callback for libcurl : write data into a memory buffer
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        printf("Not enough memory (realloc failed)\n");
        return 0;
    }
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

// Function to fetch exchange rates from the API
int fetchExchangeRates(ExchangeRates *rates, const char *api_key) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        char url[256];
        snprintf(url, sizeof(url), "https://v6.exchangerate-api.com/v6/%s/latest/EUR", api_key);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            free(chunk.memory);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return -1;
        }

        // Parse le JSON pour extraire les taux
        char *usd_rate = strstr(chunk.memory, "\"USD\":");
        char *gbp_rate = strstr(chunk.memory, "\"GBP\":");

        if (usd_rate && gbp_rate) {
            sscanf(usd_rate, "\"USD\":%lf", &rates->eur_to_usd);
            sscanf(gbp_rate, "\"GBP\":%lf", &rates->eur_to_gbp);
            rates->usd_to_eur = 1.0 / rates->eur_to_usd;
            rates->gbp_to_eur = 1.0 / rates->eur_to_gbp;
            rates->usd_to_gbp = rates->eur_to_gbp / rates->eur_to_usd;
            rates->gbp_to_usd = rates->eur_to_usd / rates->eur_to_gbp;
        } else {
            printf("Failed to parse exchange rates.\n");
            free(chunk.memory);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return -1;
        }

        free(chunk.memory);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}

// Display exchange rates
void displayExchangeRates(ExchangeRates rates) {
    printf("\n=== Current Exchange Rates ===\n");
    printf("1 EUR = %.4f USD\n", rates.eur_to_usd);
    printf("1 EUR = %.4f GBP\n", rates.eur_to_gbp);
    printf("1 USD = %.4f EUR\n", rates.usd_to_eur);
    printf("1 USD = %.4f GBP\n", rates.usd_to_gbp);
    printf("1 GBP = %.4f EUR\n", rates.gbp_to_eur);
    printf("1 GBP = %.4f USD\n", rates.gbp_to_usd);
}

// Convert currency amounts
double convertCurrency(double amount, int fromCurrency, int toCurrency, ExchangeRates rates) {
    if (fromCurrency == toCurrency) {
        return amount;
    }
    switch (fromCurrency) {
        case 1: // USD
            if (toCurrency == 2) return amount * rates.usd_to_eur; // USD -> EUR
            if (toCurrency == 3) return amount * rates.usd_to_gbp; // USD -> GBP
            break;
        case 2: // EUR
            if (toCurrency == 1) return amount * rates.eur_to_usd; // EUR -> USD
            if (toCurrency == 3) return amount * rates.eur_to_gbp; // EUR -> GBP
            break;
        case 3: // GBP
            if (toCurrency == 1) return amount * rates.gbp_to_usd; // GBP -> USD
            if (toCurrency == 2) return amount * rates.gbp_to_eur; // GBP -> EUR
            break;
    }
    return amount; // In case of error
}
