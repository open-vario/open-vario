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

#include "Stm32l476LpTimPwm.h"

#include "stm32l476xx.h"


namespace open_vario
{


/** \brief Constructor */
Stm32l476LpTimPwm::Stm32l476LpTimPwm(const Stm32l476Cpu& cpu, const LpTim lptim)
: Stm32l476LpTim(cpu, lptim)
, m_duty_cycle(0)
{}

/** \brief Configure the PWM */
bool Stm32l476LpTimPwm::configure(const uint32_t frequency, const uint8_t duty_cycle)
{
    bool ret;

    // Configure generic timer
    ret = configureTimer(frequency);
    if (ret)
    {
        // Set the duty cyle
        ret = setDutyCycle(duty_cycle);
        if (ret)
        {
            // Configure PWM mode with WAVPOL = 1 => 100% duty cycle = high level on output
            LPTIM_TypeDef* lptim = reinterpret_cast<LPTIM_TypeDef*>(getLpTimRegisters());
            uint32_t old_cr_reg_value = lptim->CR;
            lptim->CR = 0;
            lptim->CFGR &= ~(1u << 20u);
            lptim->CFGR |= (1u << 21u);
            lptim->CR = old_cr_reg_value;
        }
    }

    return ret;
}

/** \brief Start the PWM */
bool Stm32l476LpTimPwm::start()
{
    // Start generic timer
    return startTimer();
}

/** \brief Stop the PWM */
bool Stm32l476LpTimPwm::stop()
{
    // Stop generic timer
    return stopTimer();
}

/** \brief Update the frequency of the PWM */
bool Stm32l476LpTimPwm::setFrequency(const uint32_t frequency)
{
    bool ret;

    // Reset configuration
    bool was_started = isStarted();
    if (was_started)
    {
        stop();
    }
    ret = configure(frequency, m_duty_cycle);
    if (ret && was_started)
    {
        start();
    }

    return ret;
}

/** \brief Update the duty cycle of the PWM */
bool Stm32l476LpTimPwm::setDutyCycle(const uint8_t duty_cycle)
{
    LPTIM_TypeDef* lptim = reinterpret_cast<LPTIM_TypeDef*>(getLpTimRegisters());
    const uint32_t comp_value = (duty_cycle * lptim->ARR) / 100u;
    lptim->CMP = comp_value;
    m_duty_cycle = duty_cycle;
    return true;
}

}
