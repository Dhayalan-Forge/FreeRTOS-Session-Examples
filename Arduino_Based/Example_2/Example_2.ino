#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#define LED       6
#define BUTTON    8
#define SENSOR    10

TaskHandle_t ledTask, buttonTask, irTask, uartTask;

volatile uint32_t customDelay = 200;

void blinkLed(void *pvParameters)
{
  while(1)
  {
    digitalWrite(LED, HIGH);
    vTaskDelay(pdMS_TO_TICKS(customDelay));

    digitalWrite(LED, LOW);
    vTaskDelay(pdMS_TO_TICKS(customDelay));
  }
}

void readButton(void *pvParameters)
{
  while(1)
  {
    int data = digitalRead(BUTTON);
    delay(50);

    if(data == 0) Serial.println("Button Pressed");

    vTaskDelay(pdMS_TO_TICKS(100));
  }

}

void readSensor(void *pvParameters)
{
  while(1)
  {
    int data = digitalRead(SENSOR);
    delay(50);

    if(data == 0)  Serial.println("Object Detected");

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void readUART(void *pvParameters)
{
    while (1)
    {
        if (Serial.available() > 0)
        {
            int val = Serial.parseInt();

            if (val > 0 && val <= 3000)
            {
                customDelay = val;

                Serial.print("New delay: ");
                Serial.println(customDelay);
            }

            while (Serial.available() > 0)
            {
                Serial.read();
            }
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(SENSOR, INPUT);

  Serial.println("-----------------------------");

  xTaskCreate(blinkLed, "Led Task", 128, NULL, 1, &ledTask);
  xTaskCreate(readUART, "Read UART", 128, NULL, 2, &uartTask);
  xTaskCreate(readButton, "Read Button", 128, NULL, 3, &buttonTask);
  xTaskCreate(readSensor, "Read Sensor",128, NULL, 2, &irTask);

}

void loop() {

}
