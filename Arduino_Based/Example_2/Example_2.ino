#include <Arduino_FreeRTOS.h>
#include <Tasks.h>

#define LED       6
#define BUTTON    8
#define SENSOR    10

TaskHandle_t ledTask;
TaskHandle_t buttonTask;
TaskHandle_t irTask;

volatile uint32_t customDelay = 200;

void blinkLed()
{
  digitalWrite(LED, HIGH);
  vTaskDelay(pdMS_TO_TICKS(customDelay));

  digitalWrite(LED, LOW);
  vTaskDelay(pdMS_TO_TICKS(customDelay));
}

void readButton()
{
  int data = digitalRead(BUTTON);
  delay(50);

  (data == 0) ? Serial.println("Button Pressed") : NULL;
}

void readSensor()
{
  int data = digitalRead(SENSOR);
  delay(50);

  (data == 0) ? Serial.println("Object Detected") : NULL;
}

void readUART()
{
    if (Serial.available() > 0)
    {
        int val = Serial.parseInt();

        while (Serial.available())
            Serial.read();

        if (val <= 0 || val > 3000)
            val = 2000;

        customDelay = val;

        Serial.print("New delay: ");
        Serial.println(customDelay);
    }
}

void setup() {
  Serial.begin(9600);

  xTaskCreate(blinkLed, "Led Task", 128, NULL, 1, &ledTask);
  xTaskCreate(readUART, "Read UART", 128, NULL, 2, &uartTask);
  xTaskCreate(readButton, "Read Button", 128, NULL, 3, &buttonTask);
  xTaskCreate(readSensor, "Read Sensor",128, NULL, 2, &irTask);
}

void loop() {
  // put your main code here, to run repeatedly:

}
