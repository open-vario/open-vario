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

#ifndef IHWTIMER_H
#define IHWTIMER_H

#include "nano-stl.h"

namespace open_vario
{

class IHwTimer;

/** \brief Interface for all the hardware timer listeners implementations */
class IHwTimerListener
{
    public:

        /** \brief Called when the timer has elapsed */
        virtual void onTimerElapsed(IHwTimer& timer) = 0;
};


/** \brief Interface for all timers implementations */
class IHwTimer
{
    public:

        /** \brief Configure the timer */
        virtual bool configure(const uint32_t frequency, IHwTimerListener& listener) = 0;

        /** \brief Start the timer */
        virtual bool start() = 0;

        /** \brief Stop the timer */
        virtual bool stop() = 0;
};

}

#endif // IHWTIMER_H
