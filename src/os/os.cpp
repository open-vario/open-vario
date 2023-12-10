
#include "os.h"

#include "FreeRTOS.h"
#include "task.h"

namespace ov
{
namespace os
{

/** @brief Starts the operating system */
void start()
{
    // Start FreeRTOS
    vTaskStartScheduler();
}

/** @brief Yield from interrupt */
void yield_from_isr(bool higher_priority_task_woken)
{
    portYIELD_FROM_ISR(higher_priority_task_woken);
}

} // namespace os
} // namespace ov
