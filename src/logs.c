#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include "../include/logs.h"

#define LOG_FILE "logs/bank.log"

void initLogs() {
    struct stat st = {0};
    if (stat("logs", &st) == -1) {
        mkdir("logs", 0700);
    }
}

void writeLog(const char *format, ...) { // ... for variadic function
    FILE *f = fopen(LOG_FILE, "a");
    if (!f) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now); // Fixed: use localtime instead of localtime_r for C17 compliance
    fprintf(f, "<------------------------------------ ATM ------------------------------------>\n");
    fprintf(f, "[%02d-%02d-%04d %02d:%02d:%02d] ",
        t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
        t->tm_hour, t->tm_min, t->tm_sec
    );

    va_list args;
    va_start(args, format);
    vfprintf(f, format, args);
    va_end(args);

    fprintf(f, "\n");
    fclose(f);
}
