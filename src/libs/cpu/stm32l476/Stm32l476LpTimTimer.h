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

#ifndef STM32L476LPTIMTIMER_H
#define STM32L476LPTIMTIMER_H

#include "IHwTimer.h"
#include "Stm32l476LpTim.h"

namespace open_vario
{

/** \brief STM32L476 LPTIM used as a standard hardware timer */
class Stm32l476LpTimTimer : public Stm32l476LpTim, public IHwTimer
{
    public:

        /** \brief Constructor */
        Stm32l476LpTimTimer(const Stm32l476Cpu& cpu, const LpTim lptim);

        /** \brief Configure the timer */
        virtual bool configure(const uint32_t frequency, IHwTimerListener& listener);

        /** \brief Start the timer */
        virtual bool start();

        /** \brief Stop the timer */
        virtual bool stop();


    protected:

        /** \brief Called when the LPTIM interrupt has been triggered */
        virtual void onInterrupt();


    private:

        /** \brief Listener */
        IHwTimerListener* m_listener;
};

}

#endif // STM32L476LPTIMTIMER_H
