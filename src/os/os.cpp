/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

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

/** @brief Get the infinite timeout value */
uint32_t infinite_timeout_value()
{
    return portMAX_DELAY;
}

/** @brief Get the current timestamp */
uint32_t now()
{
    return xTaskGetTickCount() * portTICK_PERIOD_MS;
}

} // namespace os
} // namespace ov
