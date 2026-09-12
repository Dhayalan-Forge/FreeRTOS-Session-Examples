static SemaphoreHandle_t xBinarySemaphore = NULL;
hw_timer_t *timer = NULL;

void IRAM_ATTR onTimerISR(void)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  (void) xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void TaskA(void *pvParameters)
{
  (void) pvParameters;

  int32_t counterA = 0;
  BaseType_t semStatus;

  while(1)
  {
    semStatus = xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);

    if(semStatus == pdTRUE)
    {
      counterA++;
      (void)Serial.printf("Task A : Semaphore %ld times\n", (long)counterA );
    }
  }
}

static void TaskB(void *pvParameters)
{
  (void) pvParameters;

  int32_t counterB = 0;
  BaseType_t semStatus;

  while(1)
  {
    semStatus = xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);

    if(semStatus == pdTRUE)
    {
      counterB++;
      (void)Serial.printf("Task B : Semaphore %ld times\n", (long)counterB );
    }
  }
}

void setup(void)
{
  Serial.begin(115200);

  xBinarySemaphore = xSemaphoreCreateBinary();

  if(xBinarySemaphore != NULL)
  {
    (void) xTaskCreate(TaskA, "Task A", 2048U, NULL, 1U, NULL);
    (void) xTaskCreate(TaskB, "Task B", 2048U, NULL, 3U, NULL);

    timer = timerBegin(1000000U);
    timerAttachInterrupt(timer, &onTimerISR);
    timerAlarm(timer, 1000000U, true, 0U);
  }
}

void loop(void)
{
    /* Empty: FreeRTOS scheduler runs tasks */
}
