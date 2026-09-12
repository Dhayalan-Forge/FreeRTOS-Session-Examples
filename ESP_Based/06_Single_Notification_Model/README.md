# FreeRTOS Exercise 6: Task Notifications (Signaling a Task)

## Introduction
This exercise demonstrates the use of **Task Notifications** in FreeRTOS as a lightweight mechanism for **signaling tasks**.  
Unlike queues (which transfer data), task notifications are faster and use less memory, making them ideal for simple event signaling between tasks.

---

## FreeRTOS Notification Functions Used
- **BaseType_t xTaskNotifyGive( TaskHandle_t xTask )**  
  Sends a notification to a task. Increments the task’s internal notification counter.

- **uint32_t ulTaskNotifyTake( BaseType_t xClearCountOnExit, TickType_t xTicksToWait )**  
  Waits for a notification.  
  - `pdTRUE` clears the counter after receiving.  
  - `pdFALSE` decrements the counter.  
  - `xTicksToWait` specifies how long to block if no notification is available.

---

## Hardware/Software Requirements
- ESP32‑WROOM‑DA Module  
- Arduino IDE  
- FreeRTOS (ESP32 Arduino core)  
- Serial Monitor  

---

## Expected Output
Example Serial Monitor output:
```
Producer: Sending notification...
Consumer: Received notification!
Producer: Sending notification...
Consumer: Received notification!
```
![Serial output](./serial_output.png)
---

## Code
```ino
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

```

---

## Learning Outcomes
- Learned how to use **task notifications** for lightweight signaling between tasks.  
- Understood the difference between **queues (data transfer)** and **notifications (event signaling)**.  
- Observed how one task can **wake another task** without passing data.  
- Recognized notifications as a faster alternative to semaphores in simple signaling scenarios.

---

## Next Steps
- Explore `xTaskNotify()` and `xTaskNotifyWait()` to send values or flags along with notifications.  
- Compare notifications with **binary semaphores** (Exercise 7) to understand when each is more appropriate.  
- Extend to **event flags** using `eSetBits` for multi‑event signaling.  