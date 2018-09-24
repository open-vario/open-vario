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
    struct sigevent sigev = {0};
    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_value.sival_ptr = this;
    sigev._sigev_un._sigev_thread._function = &Timer::timerCallback;

    timer_create(CLOCK_MONOTONIC, &sigev, &m_timer);
}

/** \brief Start the timer */
bool Timer::start(ITimerListener& timer_listener, const uint32_t period)
{
    bool ret = false;

    if (m_timer != NULL)
    {
        m_listener = &timer_listener;

        struct itimerspec timerspec = {0};
    	timerspec.it_value.tv_sec = static_cast<time_t>(period / 1000u);
    	timerspec.it_value.tv_nsec = static_cast<long>((period % 1000u) * 1000000u);
    	timerspec.it_interval.tv_sec = timerspec.it_value.tv_sec;
    	timerspec.it_interval.tv_nsec = timerspec.it_value.tv_nsec;

        ret = (timer_settime(m_timer, 0, &timerspec, NULL) == 0);
    }

    return ret;
}

/** \brief Stop the timer */
bool Timer::stop()
{
    bool ret = false;
    
    if (m_timer != NULL)
    {
        struct itimerspec timerspec = {0};
        
        ret = (timer_settime(m_timer, 0, &timerspec, NULL) == 0);
    }

    return ret;
}

/** \brief Set the timer's period */
bool Timer::setPeriod(const uint32_t period)
{
    return start(*m_listener, period);
}

/** \brief Callback function of the timers */
void Timer::timerCallback(sigval_t sigval)
{
    Timer* const timer_obj = reinterpret_cast<Timer*>(sigval.sival_ptr);
    timer_obj->m_listener->timerElapsed(*timer_obj);
}


}
