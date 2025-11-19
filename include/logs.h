#ifndef LOGS_H
#define LOGS_H

void initLogs();
void writeLog(const char *format, ...);
void closeLogs();

#endif