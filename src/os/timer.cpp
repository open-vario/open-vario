

#include "timer.h"

namespace ov
{
/** @brief Constructor */
timer::timer(timer_func func, const char* name)
    : m_timer_data(), m_timer(xTimerCreateStatic(name, 1u, true, this, &timer::start_func, &m_timer_data)), m_func(func)
{
}

/** @brief Destructor */
timer::~timer()
{
    stop();
    xTimerDelete(m_timer, 0u);
}

/** @brief Start the timer */
bool timer::start(uint32_t ms_period, bool one_shot)
{
    bool ret = false;

    if (xTimerIsTimerActive(m_timer) == pdFALSE)
    {
        vTimerSetReloadMode(m_timer, static_cast<BaseType_t>(one_shot));
        xTimerChangePeriod(m_timer, ms_period / portTICK_PERIOD_MS, 0u);
        auto err = xTimerStart(m_timer, portMAX_DELAY);
        if (err == pdPASS)
        {
            ret = true;
        }
    }

    return ret;
}

/** @brief Stop the timer */
bool timer::stop()
{
    bool ret = false;

    if (xTimerIsTimerActive(m_timer) != pdFALSE)
    {
        auto err = xTimerStop(m_timer, portMAX_DELAY);
        if (err == pdPASS)
        {
            ret = true;
        }
    }

    return ret;
}

/** @brief Start function for the timer */
void timer::start_func(TimerHandle_t timer_handle)
{
    timer* t = reinterpret_cast<timer*>(pvTimerGetTimerID(timer_handle));
    if (t)
    {
        t->m_func.invoke();
    }
}

} // namespace ov
