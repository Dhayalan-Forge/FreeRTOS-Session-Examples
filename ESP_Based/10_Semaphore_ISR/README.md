# ESP32 Timer ISR with Binary Semaphore (MISRA‑C Compliant)

## Project Overview
This project demonstrates how to use an **ESP32 hardware timer interrupt** to signal tasks via a **binary semaphore** in FreeRTOS.  
Two tasks (`TaskA` and `TaskB`) wait on the same semaphore and alternate execution when the ISR gives the semaphore.  
The implementation follows **MISRA‑C guidelines** to ensure safe, predictable, and maintainable embedded code.

---

## Process Flow
1. **Timer ISR**  
   - Fires every 1 second (configurable).  
   - Signals the binary semaphore.  
   - Does not perform heavy operations (no Serial printing inside ISR).

2. **Task A**  
   - Waits indefinitely for the semaphore.  
   - Increments its own counter when unblocked.  
   - Prints its counter value to Serial.

3. **Task B**  
   - Also waits indefinitely for the semaphore.  
   - Alternates with Task A (since only one task wakes per ISR).  
   - Prints its counter value to Serial.

---

## Key Learnings
- **Binary semaphore** allows only one task to wake per ISR signal.  
- **Alternating behavior** occurs naturally when multiple tasks wait at equal priority.  
- **ISR minimalism**: keep ISRs short, only signal semaphores.  
- **FreeRTOS scheduling**: priority first, then FIFO order among equal‑priority tasks.  
- **MISRA‑C compliance** enforces safe coding practices in embedded systems.

---

## What is an ESP32 Hardware Timer?
ESP32 hardware timers are independent counters that can generate interrupts at precise intervals.  
They are widely used for periodic tasks like sensor sampling, blinking LEDs, or waking RTOS tasks.

### Benefits
- **Precise timing** independent of CPU load.  
- **ISR‑driven**: tasks wake exactly when needed.  
- **Configurable intervals** via tick frequency and alarm count.  

### Example Code Snippet
```c
timer = timerBegin(1000000U);             // 1 MHz tick (1 µs per tick)
timerAttachInterrupt(timer, &onTimerISR); // Attach ISR
timerAlarm(timer, 1000000U, true, 0U);    // Fire every 1 second
```

---

## Design Decisions
- **Binary semaphore** chosen to demonstrate alternating task wake‑up.  
- **ISR only signals**: avoids unsafe operations inside interrupt context.  
- **Task counters**: each task maintains its own counter for clarity.  
- **Different priorities**: Task B given higher priority to show scheduling impact.  
- **MISRA‑C compliance**: explicit `(void)` casts, static scope, initialized variables.

---

## MISRA‑C Introduction
MISRA‑C is a set of guidelines for writing safe, reliable, and maintainable C code in embedded systems.  
It is widely used in automotive, aerospace, and industrial applications.

### Rules Applied
1. **Rule 8.9** — Variables declared at the smallest scope.  
   - Counters inside tasks, semaphore handle `static` at file scope.  
2. **Rule 9.1** — Variables initialized before use.  
   - Counters start at 0, semaphore handle set to `NULL`.  
3. **Rule 17.7** — Functions with side effects not used in expressions.  
   - Return values stored in `semStatus` before checking.  
4. **Rule 2.7** — Parameters shall be used.  
   - `(void)pvParameters;` for unused task parameters.  
5. **Rule 21.13** — No printing inside ISR.  
   - ISR only signals semaphore.  
6. **Rule 21.3** — Explicitly discard unused return values.  
   - `(void)xTaskCreate(...)`, `(void)Serial.printf(...)`.  
7. **Rule 8.12** — Use suffixes for constants.  
   - `2048U`, `1000000U` ensure unsigned constants.

---

## Expected Output
```
Task A : Semaphore 1 times
Task B : Semaphore 1 times
Task A : Semaphore 2 times
Task B : Semaphore 2 times
...
```

---

## Conclusion
This project demonstrates how to combine **ESP32 hardware timers**, **FreeRTOS synchronization (binary semaphores)**, and **MISRA‑C compliance** to build a safe and professional embedded application.  
It shows alternating task execution driven by ISR signals, and enforces disciplined coding practices for reliability.

---