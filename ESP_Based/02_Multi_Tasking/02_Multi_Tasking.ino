const int led_pin = 16;

TaskHandle_t taskAHandle;
TaskHandle_t taskBHandle;

void led_task(void *pvParameters)
{
  pinMode(led_pin, OUTPUT);
  while(1)
  {
    digitalWrite(led_pin, HIGH);
    Serial.println("LED HIGH");
    vTaskDelay(pdMS_TO_TICKS(500));

    digitalWrite(led_pin, LOW);
    Serial.println("LED LOW");
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

/*-----------------------------------------------------------*/

void serial_print(void *pvParameters)
{
  while(1)
  {
    Serial.println("Hello From Second Task(serial_print)!!");
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

/*-----------------------------------------------------------*/

void setup() {
  Serial.begin(115200);

  if ( xTaskCreate(led_task, "Task 1", 1024, NULL, 1, &taskAHandle) != pdPASS )
  {
    Serial.println("Failed to create LED Task");
  }
  if ( xTaskCreate(serial_print, "Task 2", 1024, NULL, 1, &taskBHandle) != pdPASS )
  {
    Serial.println("Failed to create serial_print Task");
  }
}

void loop() {
// Empty: FreeRTOS scheduler runs tasks
}
