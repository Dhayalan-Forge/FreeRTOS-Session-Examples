#include "sd_logger.h"

#include <SPI.h>
#include <SD.h>

// ============================================================
// SD CARD PINS
// ============================================================

#define SD_CS_PIN    5
#define SD_SCK_PIN   18
#define SD_MISO_PIN  19
#define SD_MOSI_PIN  23

// ============================================================
// LOG FILE
// ============================================================

#define LOG_FILE "/logger.txt"

// ============================================================
// INITIALIZE SD CARD
// ============================================================

bool sdLoggerBegin()
{
    Serial.println();
    Serial.println("Initializing SD card...");

    SPI.begin(
        SD_SCK_PIN,
        SD_MISO_PIN,
        SD_MOSI_PIN,
        SD_CS_PIN
    );

    if (!SD.begin(SD_CS_PIN, SPI))
    {
        Serial.println("SD initialization failed!");

        return false;
    }

    Serial.println("SD initialization successful.");

    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card detected.");

        return false;
    }

    Serial.print("SD card type: ");

    switch (cardType)
    {
        case CARD_MMC:
            Serial.println("MMC");
            break;

        case CARD_SD:
            Serial.println("SDSC");
            break;

        case CARD_SDHC:
            Serial.println("SDHC");
            break;

        default:
            Serial.println("UNKNOWN");
            break;
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);

    Serial.print("SD card size: ");
    Serial.print(cardSize);
    Serial.println(" MB");

    return true;
}

// ============================================================
// WRITE FILE
// ============================================================

bool sdLoggerWrite(const char *message)
{
    File file = SD.open(
        LOG_FILE,
        FILE_WRITE
    );

    if (!file)
    {
        Serial.println(
            "Failed to open log file."
        );

        return false;
    }

    file.println(message);

    file.close();

    return true;
}

// ============================================================
// APPEND FILE
// ============================================================

bool sdLoggerAppend(const char *message)
{
    File file = SD.open(
        LOG_FILE,
        FILE_APPEND
    );

    if (!file)
    {
        Serial.println(
            "Failed to open log file."
        );

        return false;
    }

    file.println(message);

    file.close();

    return true;
}

// ============================================================
// READ FILE
// ============================================================

bool sdLoggerRead()
{
    File file = SD.open(
        LOG_FILE,
        FILE_READ
    );

    if (!file)
    {
        Serial.println(
            "Failed to open log file for reading."
        );

        return false;
    }

    Serial.println();
    Serial.println(
        "---------- LOG FILE ----------"
    );

    while (file.available())
    {
        Serial.write(
            file.read()
        );
    }

    Serial.println();
    Serial.println(
        "------------------------------"
    );

    file.close();

    return true;
}

// ============================================================
// SD CARD TEST
// ============================================================

void sdLoggerTest()
{
    Serial.println();
    Serial.println(
        "Testing SD card..."
    );

    if (!sdLoggerWrite(
            "ESP32 SD Logger Test"
        ))
    {
        Serial.println(
            "Write test FAILED."
        );

        return;
    }

    Serial.println(
        "Write test PASSED."
    );

    if (!sdLoggerAppend(
            "Second log entry"
        ))
    {
        Serial.println(
            "Append test FAILED."
        );

        return;
    }

    Serial.println(
        "Append test PASSED."
    );

    if (!sdLoggerRead())
    {
        Serial.println(
            "Read test FAILED."
        );

        return;
    }

    Serial.println(
        "Read test PASSED."
    );
}