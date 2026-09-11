#include <Arduino.h>

#include "sd_logger.h"

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println(
        "=============================="
    );
    Serial.println(
        "ESP32 SD CARD MODULE TEST"
    );
    Serial.println(
        "=============================="
    );

    if (!sdLoggerBegin())
    {
        Serial.println(
            "SD module initialization FAILED."
        );

        while (true)
        {
            delay(1000);
        }
    }

    sdLoggerTest();
}

void loop()
{
  delay(1000);
}