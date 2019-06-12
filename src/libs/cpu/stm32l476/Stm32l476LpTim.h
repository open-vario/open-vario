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

#ifndef STM32L476LPTIM_H
#define STM32L476LPTIM_H

#include "nano-stl.h"

namespace open_vario
{

class Stm32l476Cpu;

/** \brief IRQ handler for LPTIM 1 */
extern "C" void LPTIM1_IRQHandler(void);
/** \brief IRQ handler for LPTIM 2 */
extern "C" void LPTIM2_IRQHandler(void);



/** \brief STM32L476 LPTIM */
class Stm32l476LpTim
{
    public:


        /** \brief LPTIM number */
        enum LpTim
        {
            /** \brief LPTIM 1 */
            LPTIM_1 = 0u,
            /** \brief LPTIM 2 */
            LPTIM_2 = 1u,

            /** \brief Max LPTIM value (do not use) */
            LPTIM_MAX
        };


        /** \brief Constructor */
        Stm32l476LpTim(const Stm32l476Cpu& cpu, const LpTim lptim);

        /** \brief Configure the timer */
        bool configureTimer(const uint32_t frequency);

        /** \brief Get the frequency */
        uint32_t getFrequency() const { return m_frequency; }

        /** \brief Start the timer */
        bool startTimer();

        /** \brief Stop the timer */
        bool stopTimer();

        /** \brief Indicate is the timer is started */
        bool isStarted() const { return m_started; }


    protected:

        /** \brief Get the pointer to LPTIM registers */
        void* getLpTimRegisters();

        /** \brief Called when the LPTIM interrupt has been triggered */
        virtual void onInterrupt() {}


    private:

        /** \brief CPU */
        const Stm32l476Cpu& m_cpu;

        /** \brief LPTIM */
        const LpTim m_lptim;

        /** \brief Frequency */
        uint32_t m_frequency;

        /** \brief Indicate if the timer is started */
        bool m_started;


        /** \brief IRQ handler */
        void irqHandler();

        // To allow access to generic IRQ handler
        friend void LPTIM1_IRQHandler(void);
        friend void LPTIM2_IRQHandler(void);
};

}

#endif // STM32L476LPTIM_H
