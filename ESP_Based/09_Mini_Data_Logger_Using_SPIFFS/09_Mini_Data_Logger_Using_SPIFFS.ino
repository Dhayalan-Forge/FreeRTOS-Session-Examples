#include "FS.h"
#include "SPIFFS.h"

/*-----------------------------------------------------------
 * Configuration Constants
 * HISTORY_SIZE   : Number of recent sensor readings to store in RAM
 * PRINT_DELAY    : Interval (ms) between printer task executions
 * SENSOR_DELAY   : Interval (ms) between sensor readings
 *----------------------------------------------------------*/
#define HISTORY_SIZE      ( 5 )
#define PRINT_DELAY       ( 5000 )
#define SENSOR_DELAY      ( 1000 )

/*-----------------------------------------------------------
 * FreeRTOS Handles
 * dataQueue     : Queue for sensor data transfer
 * serialMutex   : Mutex for safe Serial access
 * sensorReadHandle, loggerHandle, printerHandle : Task handles
 *----------------------------------------------------------*/
QueueHandle_t dataQueue;
QueueHandle_t serialMutex;

TaskHandle_t sensorReadHandle;
TaskHandle_t loggerHandle;
TaskHandle_t printerHandle;

/*-----------------------------------------------------------
 * Data Structure
 * sensorData_t : Holds sensor value and timestamp
 *----------------------------------------------------------*/
typedef struct
{
  int data;
  unsigned long timeStamp;
  bool valid = false;
} sensorData_t;

/*-----------------------------------------------------------
 * Circular Buffer
 * history[]     : Stores last HISTORY_SIZE readings
 * historyIndex  : Current write position (wraps around)
 *----------------------------------------------------------*/
sensorData_t history[HISTORY_SIZE];
int historyIndex = 0;

/*-----------------------------------------------------------
 * Sensor Task
 * - Generates random sensor values every SENSOR_DELAY ms
 * - Sends data to queue
 * - Notifies logger task to process new data
 *----------------------------------------------------------*/
void sensorRead(void *pvParameters)
{
  while(1)
  {
    sensorData_t sensor;
    sensor.data = random(10,20);
    sensor.timeStamp = millis();
    sensor.valid = true;

    if ( xQueueSend(dataQueue, &sensor, portMAX_DELAY) == pdTRUE )
    {
      xTaskNotifyGive(loggerHandle);
    }
    vTaskDelay(pdMS_TO_TICKS(SENSOR_DELAY));
  }
}

/*-----------------------------------------------------------
 * Logger Task
 * - Sleeps until notified by sensor task
 * - Receives data from queue
 * - Appends entry to SPIFFS log file
 * - Updates circular buffer with latest reading
 *----------------------------------------------------------*/
void loggerTask(void *pvParameters)
{ 
  while(1)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    sensorData_t receive;
    if( xQueueReceive(dataQueue, &receive, 0) == pdTRUE && receive.valid == true )
    {
      File logFile = SPIFFS.open("/log.txt", FILE_APPEND);
      if (logFile)
      {
        logFile.printf("Sensor : %d, Timestamp : %lu ms\n", receive.data, receive.timeStamp);
        logFile.close();
      }

      history[historyIndex] = receive;
      historyIndex = (historyIndex + 1) % HISTORY_SIZE;
    }
  }
}

/*-----------------------------------------------------------
 * Printer Task
 * - Runs every PRINT_DELAY ms
 * - Prints last HISTORY_SIZE readings from circular buffer
 * - Ensures thread-safe Serial access using mutex
 * - Maintains chronological order using modular indexing
 *----------------------------------------------------------*/
void printTask(void *pvParameters)
{
  while(1)
  {
    if ( xSemaphoreTake(serialMutex, portMAX_DELAY) )
    {
      int index = historyIndex;
      Serial.println("||--------------Printing Log--------------||");
      for(int i = 0; i < HISTORY_SIZE; i++)
      {
        int position = ( index + i ) % HISTORY_SIZE;
        if ( history[position].valid == true )
        {
          Serial.printf("[Printer] Sensor : %d, Timestamp : %lu ms\n", history[position].data, history[position].timeStamp);
        }
      }
      Serial.println("||---------------Log Ended----------------||");
      xSemaphoreGive(serialMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(PRINT_DELAY));
  }
}

/*-----------------------------------------------------------
 * Setup Function
 * - Initializes Serial, random seed, queue, mutex, and SPIFFS
 * - Creates tasks with appropriate priorities
 *   Logger > Sensor > Printer
 *----------------------------------------------------------*/
void setup()
{
  Serial.begin(115200);
  randomSeed(10);

  dataQueue = xQueueCreate(10, sizeof(sensorData_t));
  if ( dataQueue == NULL )
  {
    Serial.println("Failed : Queue Creation");
    while(1);
  }

  serialMutex = xSemaphoreCreateMutex();
  if ( serialMutex == NULL )
  {
    Serial.println("Failed : Serial Mutex Creation");
    while(1);
  }

  if ( !SPIFFS.begin(true) )
  {
    Serial.println("Failed : File system access");
    while(1);
  }

  xTaskCreate(loggerTask, "Logger Task", 2048, NULL, 3, &loggerHandle);
  xTaskCreate(sensorRead, "Sensor Task", 2048, NULL, 2, &sensorReadHandle);
  xTaskCreate(printTask, "Printer Task", 2048, NULL, 1, &printerHandle);
}

/*-----------------------------------------------------------
 * Loop Function
 * - Empty because FreeRTOS scheduler manages tasks
 *----------------------------------------------------------*/
void loop()
{
  /* Empty : RTOS Scheduler takes care of tasks */
}
