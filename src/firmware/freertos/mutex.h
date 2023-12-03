
#ifndef OV_MUTEX_H
#define OV_MUTEX_H

#include "FreeRTOS.h"
#include "semphr.h"

namespace ov
{

/** @brief C++ wrapper for mutexes */
class mutex
{
  public:
    /** @brief Constructor */
    mutex();
    /** @brief Copy constructor */
    mutex(const mutex& copy) = delete;
    /** @brief Move constructor */
    mutex(mutex&& move) = delete;

    /** @brief Destructor */
    virtual ~mutex();

    /** @brief Copy operator */
    mutex& operator=(mutex& copy) = delete;

    /** @brief Lock the mutex */
    void lock();
    /** @brief Unlock the mutex */
    void unlock();

  private:
    /** @brief FreeRTOS mutex data */
    StaticSemaphore_t m_mutex_data;
    /** @brief FreeRTOS mutex handle */
    SemaphoreHandle_t m_mutex;
};

} // namespace ov

#endif // OV_MUTEX
