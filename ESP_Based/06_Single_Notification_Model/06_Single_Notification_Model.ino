TaskHandle_t consumerHandle;  /* handle for consumer task */

/* Producer Task: sends notifications */
void producerTask(void *pvParameters) {
  while (1) {
    Serial.println("Producer: Sending notification...");
    /* Notify the consumer task */
    xTaskNotifyGive(consumerHandle);
    vTaskDelay(pdMS_TO_TICKS(1000)); /* Wait 1 Second */
  }
}

/*-----------------------------------------------------------*/

/* Consumer Task: waits for notifications */
void consumerTask(void *pvParameters) {
  while (1) {
    /* Wait until a notification is received
       pdTRUE clears the count after receiving
       portMAX_DELAY blocks indefinitely until notified */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.println("Consumer: Received notification!");
  }
}

/*-----------------------------------------------------------*/

void setup() {
  Serial.begin(115200);

  /* Create consumer first so we can capture its handle */
  xTaskCreate(consumerTask, "Consumer", 2048, NULL, 1, &consumerHandle);

  /* Create producer task */
  xTaskCreate(producerTask, "Producer", 2048, NULL, 1, NULL);
}

void loop() {
  /* Empty: FreeRTOS scheduler runs tasks */
}
