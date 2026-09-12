TaskHandle_t taskAHandle;
TaskHandle_t taskBHandle;

void Core_1(void *pvParameters)
{
  while(1)
  {
    Serial.println("Hello From Core_1");
    Serial.print("Stack Usage : "); Serial.println(uxTaskGetStackHighWaterMark(NULL));
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

/*-----------------------------------------------------------*/

void Core_2(void *pvParameters)
{
  while(1)
  {
    Serial.println("Hello From Core_2");
    Serial.print("Stack Usage : "); Serial.println(uxTaskGetStackHighWaterMark(NULL));
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

/*-----------------------------------------------------------*/

void setup() {
  Serial.begin(115200);

  if ( xTaskCreatePinnedToCore(Core_1, "Core_1", 1024, NULL, 1, &taskAHandle, 0) != pdPASS )
  {
    Serial.println("Failed to create Core_1 Task");
  }
  if ( xTaskCreatePinnedToCore(Core_2, "Core_2", 1024, NULL, 3, &taskBHandle, 1) != pdPASS )
  {
    Serial.println("Failed to create Core_2 Task");
  }
}

void loop() {
// Empty: FreeRTOS scheduler runs tasks
}
