QueueHandle_t serialMutex;
TaskHandle_t taskAHandle;
TaskHandle_t taskBHandle;

void TaskA(void *pvParameters)
{
  while(1)
  {
    static int counter = 0;
    if ( xSemaphoreTake(serialMutex, portMAX_DELAY) )
    {
      Serial.println("Task A : Notification");
      Serial.print("Task A Priority: ");
      Serial.println(uxTaskPriorityGet(taskAHandle));
      xSemaphoreGive(serialMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(1000));

    counter++;
    if (counter == 5)
    {
      Serial.println("Changing Task B Priority");
      /* Changing the priority of Task B */
      vTaskPrioritySet(taskBHandle, 3);
    }
  }
}

void TaskB(void *pvParameters)
{
  while(1)
  {
    if ( xSemaphoreTake(serialMutex, portMAX_DELAY) )
    {
      Serial.println("Task B : Notification");
      Serial.print("Task B Priority: ");
      Serial.println(uxTaskPriorityGet(taskBHandle));
      xSemaphoreGive(serialMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {
  Serial.begin(115200);

  serialMutex = xSemaphoreCreateMutex();
  if ( serialMutex == NULL )
  {
    Serial.println("Failed : Mutex Creation");
    while(1);
  }

  xTaskCreate(TaskA, "Task A", 2048, NULL, 1, &taskAHandle);
  xTaskCreate(TaskB, "Task B", 2048, NULL, 1, &taskBHandle);

}

void loop() {
  /* Empty: FreeRTOS scheduler runs tasks */
}
