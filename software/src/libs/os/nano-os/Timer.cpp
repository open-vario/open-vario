/*
Copyright(c) 2017 Cedric Jimenez

This file is part of Open-Vario.

Open-Vario is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Open-Vario is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Open-Vario.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Timer.h"

namespace open_vario
{


/** \brief Constructor */
Timer::Timer()
: m_timer()
, m_listener(NULL)
, m_data(NULL)
{
    NANO_OS_TIMER_Create(&m_timer, &Timer::timerCallback, this);
}

/** \brief Start the timer */
bool Timer::start(ITimerListener& timer_listener, const uint32_t period)
{
    m_listener = &timer_listener;
    const nano_os_error_t ret = NANO_OS_TIMER_Start(&m_timer, NANO_OS_MS_TO_TICKS(period), NANO_OS_MS_TO_TICKS(period));
    return (ret == NOS_ERR_SUCCESS);
}

/** \brief Stop the timer */
bool Timer::stop()
{
    const nano_os_error_t ret = NANO_OS_TIMER_Stop(&m_timer);
    return (ret == NOS_ERR_SUCCESS);
}

/** \brief Set the timer's period */
bool Timer::setPeriod(const uint32_t period)
{
    bool ret = false;
    
    if (stop())
    {
        ret = start(*m_listener, period);
    }
    
    return ret;
}

/** \brief Callback function of the timers */
void Timer::timerCallback(nano_os_timer_t* const timer, void* const user_data)
{
    Timer* const timer_obj = reinterpret_cast<Timer*>(user_data);
    timer_obj->m_listener->timerElapsed(*timer_obj);
}


}
