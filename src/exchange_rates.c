#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "../include/bank.h"

#define EXCHANGE_RATES_FILE "exchange_rates.dat"

struct MemoryStruct {
    char *memory;
    size_t size;
};

// Callback pour curl
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

// Sauvegarde des taux de change dans un fichier
void saveExchangeRates(ExchangeRates rates) {
    FILE *file = fopen(EXCHANGE_RATES_FILE, "w");
    if (!file) {
        printf("Error saving exchange rates!\n");
        return;
    }

    fprintf(file, "EUR TO USD: %.6f\n", rates.eur_to_usd);
    fprintf(file, "EUR TO GBP: %.6f\n", rates.eur_to_gbp);
    fprintf(file, "USD TO EUR: %.6f\n", rates.usd_to_eur);
    fprintf(file, "USD TO GBP: %.6f\n", rates.usd_to_gbp);
    fprintf(file, "GBP TO EUR: %.6f\n", rates.gbp_to_eur);
    fprintf(file, "GBP TO USD: %.6f\n", rates.gbp_to_usd);

    fclose(file);
    printf("Exchange rates saved to file.\n");
}

// Chargement des taux de change depuis le fichier
int loadExchangeRates(ExchangeRates *rates) {
    FILE *file = fopen(EXCHANGE_RATES_FILE, "r");
    if (!file) return -1;

    char line[128];
    if (fgets(line, sizeof(line), file)) sscanf(line, "EUR TO USD: %lf", &rates->eur_to_usd);
    if (fgets(line, sizeof(line), file)) sscanf(line, "EUR TO GBP: %lf", &rates->eur_to_gbp);
    if (fgets(line, sizeof(line), file)) sscanf(line, "USD TO EUR: %lf", &rates->usd_to_eur);
    if (fgets(line, sizeof(line), file)) sscanf(line, "USD TO GBP: %lf", &rates->usd_to_gbp);
    if (fgets(line, sizeof(line), file)) sscanf(line, "GBP TO EUR: %lf", &rates->gbp_to_eur);
    if (fgets(line, sizeof(line), file)) sscanf(line, "GBP TO USD: %lf", &rates->gbp_to_usd);

    fclose(file);
    return 0;
}

// Récupération des taux de change via l'API
int fetchExchangeRates(ExchangeRates *rates, const char *api_key) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl) {
        printf("Failed to initialize curl.\n");
        free(chunk.memory);
        curl_global_cleanup();
        return -1;
    }

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

    // Affichage de la réponse brute pour debug
    printf("API Response: %s\n", chunk.memory);

    // Parsing basique du JSON
    char *usd_rate = strstr(chunk.memory, "\"USD\":");
    char *gbp_rate = strstr(chunk.memory, "\"GBP\":");

    if (usd_rate && gbp_rate) {
        sscanf(usd_rate, "\"USD\":%lf", &rates->eur_to_usd);
        sscanf(gbp_rate, "\"GBP\":%lf", &rates->eur_to_gbp);

        // Calcul des taux croisés
        rates->usd_to_eur = 1.0 / rates->eur_to_usd;
        rates->gbp_to_eur = 1.0 / rates->eur_to_gbp;
        rates->usd_to_gbp = rates->eur_to_gbp / rates->eur_to_usd;
        rates->gbp_to_usd = rates->eur_to_usd / rates->eur_to_gbp;

        saveExchangeRates(*rates);
        printf("Exchange rates fetched and saved.\n");
    } else {
        printf("Failed to parse exchange rates.\n");
        free(chunk.memory);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return -1;
    }

    free(chunk.memory);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
}

// Affichage des taux de change
void displayExchangeRates(ExchangeRates rates) {
    printf("\n=== Current Exchange Rates ===\n");
    printf("1 EUR = %.4f USD\n", rates.eur_to_usd);
    printf("1 EUR = %.4f GBP\n", rates.eur_to_gbp);
    printf("1 USD = %.4f EUR\n", rates.usd_to_eur);
    printf("1 USD = %.4f GBP\n", rates.usd_to_gbp);
    printf("1 GBP = %.4f EUR\n", rates.gbp_to_eur);
    printf("1 GBP = %.4f USD\n", rates.gbp_to_usd);
}

// Conversion entre devises
double convertCurrency(double amount, Currency fromCurrency, Currency toCurrency, ExchangeRates rates) {
    if (fromCurrency == toCurrency) return amount;

    switch (fromCurrency) {
        case USD:
            if (toCurrency == EUR) return amount * rates.usd_to_eur;
            if (toCurrency == GBP) return amount * rates.usd_to_gbp;
            break;
        case EUR:
            if (toCurrency == USD) return amount * rates.eur_to_usd;
            if (toCurrency == GBP) return amount * rates.eur_to_gbp;
            break;
        case GBP:
            if (toCurrency == USD) return amount * rates.gbp_to_usd;
            if (toCurrency == EUR) return amount * rates.gbp_to_eur;
            break;
    }
    return amount;
}
