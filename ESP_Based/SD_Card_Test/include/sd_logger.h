#ifndef SD_LOGGER_H
#define SD_LOGGER_H

#include <Arduino.h>

bool sdLoggerBegin();

bool sdLoggerWrite(
    const char *message
);

bool sdLoggerRead();

bool sdLoggerAppend(
    const char *message
);

void sdLoggerTest();

#endif