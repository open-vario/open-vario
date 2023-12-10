
#include "FreeRTOS.h"
#include "task.h"

#include <stdbool.h>

/** @brief Idle task's TCB */
static StaticTask_t s_idle_task_tcb;
/** @brief Idle task's size */
static StackType_t s_idle_task_stack[configMINIMAL_STACK_SIZE];

/** @brief This function is used to provide a statically allocated block of memory to FreeRTOS to hold the Idle Task TCB.  This function is required when
 *         configSUPPORT_STATIC_ALLOCATION is set */
void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
                                   StackType_t**  ppxIdleTaskStackBuffer,
                                   uint32_t*      pulIdleTaskStackSize)
{
    (*ppxIdleTaskTCBBuffer)   = &s_idle_task_tcb;
    (*ppxIdleTaskStackBuffer) = &s_idle_task_stack[0];
    (*pulIdleTaskStackSize)   = (sizeof(s_idle_task_stack) / sizeof(StackType_t));
}

/** @brief Timer task's TCB */
static StaticTask_t s_timer_task_tcb;
/** @brief Timer task's size */
static StackType_t s_timer_task_stack[configTIMER_TASK_STACK_DEPTH];

/** @brief This function is used to provide a statically allocated block of memory to FreeRTOS to hold the Timer Task TCB.  This function is required when
 *         configSUPPORT_STATIC_ALLOCATION is set */
void vApplicationGetTimerTaskMemory(StaticTask_t** ppxTimerTaskTCBBuffer,
                                    StackType_t**  ppxTimerTaskStackBuffer,
                                    uint32_t*      pulTimerTaskStackSize)
{
    (*ppxTimerTaskTCBBuffer)   = &s_timer_task_tcb;
    (*ppxTimerTaskStackBuffer) = &s_timer_task_stack[0];
    (*pulTimerTaskStackSize)   = (sizeof(s_timer_task_stack) / sizeof(StackType_t));
}

/** @brief The application stack overflow hook is called when a stack overflow is detected for a task */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;
    taskDISABLE_INTERRUPTS();
    while (true)
    {
        // Wait for watchdog reset
    }
}
