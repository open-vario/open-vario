
#include "semaphore.h"

namespace ov
{

/** @brief Constructor */
semaphore::semaphore(uint32_t init_count, uint32_t max_count)
    : m_semaphore_data(), m_semaphore(xSemaphoreCreateCountingStatic(max_count, init_count, &m_semaphore_data))
{
}

/** @brief Destructor */
semaphore::~semaphore()
{
    vSemaphoreDelete(m_semaphore);
}

/** @brief Take a token from the semaphore */
bool semaphore::take()
{
    auto ret = xSemaphoreTake(m_semaphore, portMAX_DELAY);
    return (ret == pdTRUE);
}

/** @brief Take a token from the semaphore with a timeout */
bool semaphore::take(uint32_t ms_timeout)
{
    auto ret = xSemaphoreTake(m_semaphore, ms_timeout / portTICK_PERIOD_MS);
    return (ret == pdTRUE);
}

/** @brief Release a token from the semaphore */
bool semaphore::release()
{
    auto ret = xSemaphoreGive(m_semaphore);
    return (ret == pdTRUE);
}

/** @brief Release a token from the semaphore from an ISR */
bool semaphore::release_from_isr(bool& higher_priority_task_woken)
{
    bool       ret = false;
    BaseType_t task_woken;

    if (xSemaphoreGiveFromISR(m_semaphore, &task_woken) == pdTRUE)
    {
        higher_priority_task_woken = (task_woken == pdTRUE);
        ret                        = true;
    }

    return ret;
}

} // namespace ov
