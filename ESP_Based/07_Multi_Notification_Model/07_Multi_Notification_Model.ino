TaskHandle_t taskHandleBinary;
TaskHandle_t taskHandleValue;
TaskHandle_t taskHandleCounter;
TaskHandle_t taskHandleFlag;

/* Producer for binary notification */
void producerBinary(void *pvParameters)
{
  while(1)
  {
    Serial.println("Binary: Sending Notification..");
    xTaskNotifyGive(taskHandleBinary);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

/* Consumer for binary notification */
void consumerBinary(void *pvParameters)
{
  while(1)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.println("Binary: Received Notification!!");
  }
}

/*------------------------------------------------------------*/

/* Producer for overwrite value */
void producerValue(void *pvParameters)
{
  uint32_t status = 0;
  while(1)
  {
    status = ( status + 1 ) % 3;
    Serial.print("Value Producer: sending status ");
    Serial.println(status);
    xTaskNotify(taskHandleValue, status, eSetValueWithOverwrite);
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

/* Consumer for overwrite value */
void consumerValue(void *pvParameters)
{
  uint32_t received;
  while(1)
  {
    if ( xTaskNotifyWait(0, 0, &received, portMAX_DELAY ) == pdTRUE )
    {
      Serial.print("Value Consumer: received status : ");
      Serial.println(received);
    }
  }
}

/*------------------------------------------------------------*/

/* Producer for increment counter */
void producerCounter(void *pvParameters)
{
  while(1)
  {
    Serial.println("Counter Producer: incrementing value...");
    xTaskNotify(taskHandleCounter, 0, eIncrement);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

/* Consumer for increment counter */
void consumerCounter(void *pvParameters)
{
  while(1)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.println("Counter Consumer: processed one event!");
  }
}

/*-----------------------------------------------------------*/

/* Producer for flags */
void producerFlag(void *pvParameters)
{
  uint32_t flag = 1;
  while(1)
  {
    Serial.print("Flag producer: setting flag ");
    Serial.println(flag);
    xTaskNotify(taskHandleFlag, flag, eSetBits);
    flag <<= 1;
    /* Resets the flag to 1 after cycle through 1,2,4 */
    if ( flag > 0x04 ) flag = 1; 
    vTaskDelay(pdMS_TO_TICKS(1500));
  }
}

/* Consumer for flags */
void consumerFlag(void *pvParameters)
{
  uint32_t flags;
  while(1)
  {
    if ( xTaskNotifyWait(0, 0, &flags, portMAX_DELAY) == pdPASS )
    {
      Serial.print("Flags consumer: got flag ");
      Serial.println(flags, BIN);
    }
  }
}
/*-----------------------------------------------------------*/

void setup() 
{
  Serial.begin(115200);

  xTaskCreate(consumerBinary, "BinaryConsumer", 2048, NULL, 1, &taskHandleBinary);
  xTaskCreate(producerBinary, "BinaryProducer", 2048, NULL, 1, NULL);

  xTaskCreate(consumerValue, "ValueConsumer", 2048, NULL, 1, &taskHandleValue);
  xTaskCreate(producerValue, "ValueProducer", 2048, NULL, 1, NULL);

  xTaskCreate(consumerCounter, "CounterConsumer", 2048, NULL, 1, &taskHandleCounter);
  xTaskCreate(producerCounter, "CounterProducer", 2048, NULL, 1, NULL);

  xTaskCreate(consumerFlag, "FlagsConsumer", 2048, NULL, 1, &taskHandleFlag);
  xTaskCreate(producerFlag, "FlagsProducer", 2048, NULL, 1, NULL);
}

void loop() 
{
  /* Empty: FreeRTOS scheduler runs tasks */
}
