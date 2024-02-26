/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "mutex.h"

namespace ov
{

/** @brief Constructor */
mutex::mutex() : m_mutex_data(), m_mutex(xSemaphoreCreateMutexStatic(&m_mutex_data)) { }

/** @brief Destructor */
mutex::~mutex()
{
    vSemaphoreDelete(m_mutex);
}

/** @brief Lock the mutex */
void mutex::lock()
{
    if (xSemaphoreTake(m_mutex, portMAX_DELAY) != pdTRUE)
    {
        while (true)
            ;
    }
}

/** @brief Unlock the mutex */
void mutex::unlock()
{
    if (xSemaphoreGive(m_mutex) != pdTRUE)
    {
        while (true)
            ;
    }
}

} // namespace ov
