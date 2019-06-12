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

#include "Stm32l476LpTimTimer.h"

#include "stm32l476xx.h"


namespace open_vario
{


/** \brief Constructor */
Stm32l476LpTimTimer::Stm32l476LpTimTimer(const Stm32l476Cpu& cpu, const LpTim lptim)
: Stm32l476LpTim(cpu, lptim)
, m_listener(nullptr)
{}

/** \brief Configure the timer */
bool Stm32l476LpTimTimer::configure(const uint32_t frequency, IHwTimerListener& listener)
{
    bool ret;

    // Configure generic timer
    ret = configureTimer(frequency);
    if (ret)
    {
        // Save listener
        m_listener = &listener;

        // Enable match interrupt
        LPTIM_TypeDef* lptim = reinterpret_cast<LPTIM_TypeDef*>(getLpTimRegisters());
        uint32_t old_cr_reg_value = lptim->CR;
        lptim->CR = 0;
        lptim->IER |= (1u << 1u);
        lptim->CR = old_cr_reg_value;
    }

    return ret;
}

/** \brief Start the timer */
bool Stm32l476LpTimTimer::start()
{
    // Start generic timer
    return startTimer();
}

/** \brief Stop the timer */
bool Stm32l476LpTimTimer::stop()
{
    // Stop generic timer
    return stopTimer();
}

/** \brief Called when the LPTIM interrupt has been triggered */
void Stm32l476LpTimTimer::onInterrupt()
{
    // Call the listener
    m_listener->onTimerElapsed(*this);
}

}
