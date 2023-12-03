
#ifndef OV_SEMAPHORE_H
#define OV_SEMAPHORE_H

#include "FreeRTOS.h"
#include "semphr.h"

namespace ov
{

/** @brief C++ wrapper for semaphores */
class semaphore
{
  public:
    /** @brief Constructor */
    semaphore(uint32_t init_count, uint32_t max_count);
    /** @brief Copy constructor */
    semaphore(const semaphore& copy) = delete;
    /** @brief Move constructor */
    semaphore(semaphore&& move) = delete;

    /** @brief Destructor */
    virtual ~semaphore();

    /** @brief Copy operator */
    semaphore& operator=(semaphore& copy) = delete;

    /** @brief Take a token from the semaphore */
    bool take();
    /** @brief Take a token from the semaphore with a timeout */
    bool take(uint32_t ms_timeout);
    /** @brief Release a token from the semaphore */
    bool release();
    /** @brief Release a token from the semaphore from an ISR */
    bool release_from_isr(bool& higher_priority_task_woken);

  private:
    /** @brief FreeRTOS semaphore data */
    StaticSemaphore_t m_semaphore_data;
    /** @brief FreeRTOS semaphore handle */
    SemaphoreHandle_t m_semaphore;
};

} // namespace ov

#endif // OV_SEMAPHORE_H
