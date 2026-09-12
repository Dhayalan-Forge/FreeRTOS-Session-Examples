# FreeRTOS Exercise 1: Single Task LED Blink

## Introduction
This example demonstrates single task creation in FreeRTOS. The task blinks an LED connected to pin 16 on the ESP32‑WROOM‑DA module.

## Commands Used
- **xTaskCreate()** → Creates a new task and adds it to the scheduler.
- **vTaskDelay()** → Suspends the task for a given time, allowing other tasks to run.
- **pdMS_TO_TICKS()** → Converts milliseconds to RTOS ticks for accurate timing.

## Hardware/Software Requirements
- ESP32‑WROOM‑DA Module
- Arduino IDE
- FreeRTOS (ESP32 Arduino core)
- Serial Monitor

## Expected Output
```
LED High!!
LED Low!!
LED High!!
LED Low!!
```

## Code
```ino
#define LED_PIN 16
TaskHandle_t taskAHandle;

void TaskA(void *pvParameters)
{
  while(1)
  {
    Serial.println("LED High!!");
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(200));

    Serial.println("LED Low!!");
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  xTaskCreate(TaskA, "TaskA", 1024, NULL, 1, &taskAHandle);

}

void loop() {

}
```
## Learning Outcomes
- Learned how to create a FreeRTOS task.
- Understood how tasks yield CPU time.
- Observed difference between Arduino loop and FreeRTOS task scheduling.