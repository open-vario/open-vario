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

#ifndef STM32L476CPU_H
#define STM32L476CPU_H

#include "ICpu.h"

namespace open_vario
{



/** \brief STM32L476 CPU */
class Stm32l476Cpu : public ICpu
{
    public:

        /** \brief Constructor */
        Stm32l476Cpu();

        /** \brief Get the CPU's name */
        virtual const char* name() const { return "STM32L476RE"; }

        /** \brief Get the CPU's frequency in Hz */
        virtual uint32_t getFrequency() const { return 48000000u; /* To be changed with runtime computation */}

        /** \brief Get the CPU's RAM size in bytes */
        virtual uint32_t ramSize() const { return 0x00008000u; }

        /** \brief Get the CPU's ROM size in bytes */
        virtual uint32_t romSize() const { return 0x00018000u; }


        /** \brief Reset the CPU */
        virtual bool reset();


        /** \brief Get the SYSCLK frequency */
        virtual uint32_t getSysclkFrequency() const { return 48000000u; /* To be changed with runtime computation */}

        /** \brief Get the HCLK frequency */
        virtual uint32_t getHclkFrequency() const { return 48000000u; /* To be changed with runtime computation */}

        /** \brief Get the PCLK1 frequency */
        virtual uint32_t getPclk1Frequency() const { return 12000000u; /* To be changed with runtime computation */}

        /** \brief Get the PCLK2 frequency */
        virtual uint32_t getPclk2Frequency() const { return 12000000u; /* To be changed with runtime computation */}
};

}

#endif // STM32L476CPU_H
