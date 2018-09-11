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

#ifndef ITIMER_H
#define ITIMER_H

#include <cstdint>

namespace open_vario
{

// Timer interface forward declaration
class ITimer;

/** \brief Interface for all classes which implements a timer callback method */
class ITimerListener
{
    public:

        /** \brief Method which will be called when the timer elapsed */
        virtual void timerElapsed(ITimer& timer) = 0;
};


/** \brief Interface for all timer implementations */
class ITimer
{
    public:

        /** \brief Start the timer */
        virtual bool start(ITimerListener& timer_listener, const uint32_t period) = 0;

        /** \brief Stop the timer */
        virtual bool stop() = 0;

        /** \brief Set the timer's period */
        virtual bool setPeriod(const uint32_t period) = 0;

        /** \brief Set the timer's associated data */
        virtual void setData(void* const data) = 0;

        /** \brief Get the timer's associated data */
        virtual void* getData() const = 0;

};


}

#endif // ITIMER_H
