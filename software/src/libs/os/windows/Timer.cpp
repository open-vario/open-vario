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
: m_timer(NULL)
, m_listener(NULL)
, m_data(NULL)
{
    m_timer = CreateThreadpoolTimer(&Timer::timerCallback, this, NULL);
}

/** \brief Start the timer */
bool Timer::start(ITimerListener& timer_listener, const uint32_t period)
{
    bool ret = false;

    if (m_timer != NULL)
    {
        m_listener = &timer_listener;
        LONGLONG due_time_long = -10000 * static_cast<LONGLONG>(period);
        SetThreadpoolTimer(m_timer, reinterpret_cast<FILETIME*>(&due_time_long), period, 0);

        ret = true;
    }

    return ret;
}

/** \brief Stop the timer */
bool Timer::stop()
{
    bool ret = false;
    
    if (m_timer != NULL)
    {
        SetThreadpoolTimer(m_timer, NULL, 0, 0);

        ret = true;
    }

    return ret;
}

/** \brief Set the timer's period */
bool Timer::setPeriod(const uint32_t period)
{
    return start(*m_listener, period);
}

/** \brief Callback function of the timers */
void CALLBACK Timer::timerCallback(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_TIMER tim)
{
    (void)instance;
    (void)tim;
    Timer* const timer_obj = reinterpret_cast<Timer*>(context);
    timer_obj->m_listener->timerElapsed(*timer_obj);
}


}
