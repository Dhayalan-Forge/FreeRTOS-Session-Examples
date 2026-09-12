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
