# Sensor Logger with FreeRTOS, SPIFFS, and Circular Buffer

## Project Overview
This project demonstrates a **multi‑tasking embedded application** using FreeRTOS on ESP32.  
It simulates sensor data, logs values to SPIFFS, and periodically prints the latest readings using a **circular buffer**.

### Process Flow
1. **Sensor Task**  
   - Generates random sensor values every second (`SENSOR_DELAY`).  
   - Sends data to a FreeRTOS queue.  
   - Notifies the logger task.

2. **Logger Task**  
   - Sleeps until notified.  
   - Receives data from the queue.  
   - Writes each entry to SPIFFS (`/log.txt`).  
   - Stores the latest N values in a circular buffer (`HISTORY_SIZE`).

3. **Printer Task**  
   - Runs every 5 seconds (`PRINT_DELAY`).  
   - Prints the last N values from the circular buffer.  
   - Ensures thread‑safe access with a mutex.

---

## Key Learnings
- **Queues** handle producer–consumer communication.  
- **Task notifications** wake tasks efficiently, making the system event‑driven.  
- **SPIFFS** provides persistent storage for logs.  
- **Circular buffers** allow efficient storage of the latest N values without shifting arrays.  
- **Mutexes** ensure safe access to shared resources like Serial.

---

## What is a Circular Buffer?
A circular buffer (ring buffer) is a fixed‑size array that overwrites old data when full.  
It’s widely used in embedded systems for UART drivers, logging, and streaming data.

### Benefits
- **Constant memory usage** (no dynamic allocation).  
- **Efficient** (no shifting elements).  
- **Always keeps the latest N values**.  

### Example Code Snippet
```ino
#define HISTORY_SIZE 5
sensorData_t history[HISTORY_SIZE];
int historyIndex = 0;

// In loggerTask after writing to SPIFFS:
history[historyIndex] = receive;
historyIndex = (historyIndex + 1) % HISTORY_SIZE;

// In printTask:
int index = historyIndex;
for(int i = 0; i < HISTORY_SIZE; i++)
{
    int position = ( index + i ) % HISTORY_SIZE;
    Serial.printf("[Printer] Sensor : %d, Timestamp : %lu ms\n", history[position].data, history[position].timeStamp);
}
```

### Example Output
```
||--------------Printing Log--------------||
[Printer] Sensor : 11, Timestamp : 23314 ms
[Printer] Sensor : 13, Timestamp : 24314 ms
[Printer] Sensor : 15, Timestamp : 25314 ms
[Printer] Sensor : 18, Timestamp : 26314 ms
[Printer] Sensor : 17, Timestamp : 27314 ms
||---------------Log Ended----------------||
```
Here, the buffer rotates so the oldest entry is printed first, and the newest last.

---

## Design Decisions
- **Queue + Notification:** Queue for data transfer, notification for explicit wake‑up.  
- **Logger Priority Higher than Sensor:** Ensures immediate processing when notified.  
- **Circular Buffer in RAM:** For fast access to recent values.  
- **SPIFFS Logging:** For persistence and debugging.  
- **Mutex for Serial:** Prevents race conditions when multiple tasks print.  
- **Configurable Constants:** `HISTORY_SIZE`, `PRINT_DELAY`, and `SENSOR_DELAY` defined at the top for easy tuning.

---

## Expected Output
- Sensor values generated every second.  
- Logger writes them to SPIFFS and circular buffer.  
- Printer shows the last 5 readings every 5 seconds.  
- Example:
  ```
  ||--------------Printing Log--------------||
  [Printer] Sensor : 12, Timestamp : 14315 ms
  [Printer] Sensor : 19, Timestamp : 15315 ms
  [Printer] Sensor : 10, Timestamp : 16315 ms
  [Printer] Sensor : 17, Timestamp : 17315 ms
  [Printer] Sensor : 14, Timestamp : 18315 ms
  ||---------------Log Ended----------------||
  ```

---

## Conclusion
This project demonstrates how to combine **RTOS concepts (queues, notifications, mutexes)** with **persistent storage (SPIFFS)** and **efficient data structures (circular buffer)** to build a professional embedded logging system.

---