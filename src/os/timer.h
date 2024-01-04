
#ifndef OV_TIMER_H
#define OV_TIMER_H

#include "FreeRTOS.h"
#include "timers.h"

#include "delegate.h"

namespace ov
{

/** @brief Thread function */
using timer_func = delegate<void>;

/** @brief C++ wrapper for timers */
class timer
{
  public:
    /** @brief Constructor */
    timer(timer_func func, const char* name);
    /** @brief Copy constructor */
    timer(const timer& copy) = delete;
    /** @brief Move constructor */
    timer(timer&& move) = delete;

    /** @brief Destructor */
    virtual ~timer();

    /** @brief Copy operator */
    timer& operator=(timer& copy) = delete;

    /** @brief Start the timer */
    bool start(uint32_t ms_period, bool one_shot = false);

    /** @brief Stop the timer */
    bool stop();

  private:
    /** @brief FreeRTOS timer data */
    StaticTimer_t m_timer_data;
    /** @brief FreeRTOS timer handle */
    TimerHandle_t m_timer;
    /** @brief Delegate for the timer function */
    timer_func m_func;

    /** @brief Start function for the timer */
    static void start_func(TimerHandle_t timer_handle);
};

} // namespace ov

#endif // OV_TIMER_H
