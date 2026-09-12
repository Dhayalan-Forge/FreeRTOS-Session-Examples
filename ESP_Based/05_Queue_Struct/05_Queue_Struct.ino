/*
  FreeRTOS Queue Functions Used:

  QueueHandle_t xQueueCreate( UBaseType_t uxQueueLength, UBaseType_t uxItemSize );
    - Creates a queue with specified length and item size.
    - Returns a handle to the created queue.

  BaseType_t xQueueSend( QueueHandle_t xQueue, const void * pvItemToQueue, TickType_t xTicksToWait );
    - Sends an item to the back of the queue.
    - Blocks for xTicksToWait if the queue is full.
    - Returns pdPASS if successful.

  BaseType_t xQueueReceive( QueueHandle_t xQueue, void * pvBuffer, TickType_t xTicksToWait );
    - Receives an item from the queue.
    - Blocks for xTicksToWait if the queue is empty.
    - Returns pdPASS if successful.

  void vQueueDelete( QueueHandle_t xQueue );
    - Deletes a queue (not used in this example).
*/

QueueHandle_t queue1;

typedef struct 
{
  int ID;
  int value;
} sensor_t;

char buffer[64];

/* Producer Task: generates sensor data and sends to queue */
void producerTask(void *pvParameters) 
{
  sensor_t producerData;
  producerData.ID = 1;
  producerData.value = 0;

  while (1) 
  {
    producerData.value += 10;
    if (xQueueSend(queue1, &producerData, portMAX_DELAY) == pdPASS) 
    {
      snprintf(buffer, sizeof(buffer), "Sent - ID : %d, Value : %d", producerData.ID, producerData.value);
      Serial.println(buffer);
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

/*-----------------------------------------------------------*/

/* Consumer Task: receives sensor data from queue and prints */
void consumerTask(void *pvParameters) 
{
  sensor_t consumerData;
  while (1) 
  {
    if (xQueueReceive(queue1, &consumerData, portMAX_DELAY) == pdPASS) 
    {
      snprintf(buffer, sizeof(buffer), "Received - ID : %d, Value : %d", consumerData.ID, consumerData.value);
      Serial.println(buffer);
    }
  }
}

/*-----------------------------------------------------------*/

void setup() {
  Serial.begin(115200);

  // Create a queue to hold 5 sensor_t items
  queue1 = xQueueCreate(5, sizeof(sensor_t));
  if (queue1 == NULL) {
    Serial.println("Failed to create Queue..");
    while (1);
  }

  xTaskCreate(producerTask, "Producer", 2048, NULL, 1, NULL);
  xTaskCreate(consumerTask, "Consumer", 2048, NULL, 1, NULL);
}

void loop() {
  // Empty: FreeRTOS scheduler runs tasks
}
