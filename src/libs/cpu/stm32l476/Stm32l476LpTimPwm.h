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

#ifndef STM32L476LPTIMPWM_H
#define STM32L476LPTIMPWM_H

#include "IPwm.h"
#include "Stm32l476LpTim.h"

namespace open_vario
{


/** \brief STM32L476 LPTIM used as a PWM generator */
class Stm32l476LpTimPwm : public Stm32l476LpTim, public IPwm
{
    public:

        /** \brief Constructor */
        Stm32l476LpTimPwm(const Stm32l476Cpu& cpu, const LpTim lptim);

        /** \brief Configure the PWM */
        virtual bool configure(const uint32_t frequency, const uint8_t duty_cycle);

        /** \brief Start the PWM */
        virtual bool start();

        /** \brief Stop the PWM */
        virtual bool stop();

        /** \brief Update the frequency of the PWM */
        virtual bool setFrequency(const uint32_t frequency);

        /** \brief Update the duty cycle of the PWM */
        virtual bool setDutyCycle(const uint8_t duty_cycle);


    private:

        /** \brief Duty cycle */
        uint8_t m_duty_cycle;
};

}

#endif // STM32L476LPTIMPWM_H
