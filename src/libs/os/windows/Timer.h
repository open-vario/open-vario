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

#ifndef TIMER_H
#define TIMER_H

#include "ITimer.h"

#include <Windows.h>

namespace open_vario
{


/** \brief Timer implementation for Windows */
class Timer : public ITimer
{
    public:

        /** \brief Constructor */
        Timer();


        /** \brief Start the timer */
        virtual bool start(ITimerListener& timer_listener, const uint32_t period);

        /** \brief Stop the timer */
        virtual bool stop();

        /** \brief Set the timer's period */
        virtual bool setPeriod(const uint32_t period);

        /** \brief Set the timer's associated data */
        virtual void setData(void* const data) { m_data = data; }

        /** \brief Get the timer's associated data */
        virtual void* getData() const { return m_data; }


    private:

        /** \brief Underlying Windows timer */
        PTP_TIMER m_timer;

        /** \brief Timer listener */
        ITimerListener* m_listener;

        /** \brief Timer data */
        void* m_data;


        /** \brief Callback function of the timers */
        static void CALLBACK timerCallback(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_TIMER tim);
};


}

#endif // TIMER_H
