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

#include "Stm32l476Cpu.h"

#include "stm32l476xx.h"

namespace open_vario
{


/** \brief Constructor */
Stm32l476Cpu::Stm32l476Cpu()
{}

/** \brief Reset the CPU */
bool Stm32l476Cpu::reset()
{
    // Reset CPU using NVIC
    NVIC_SystemReset();

    // If we get here then something went wrong!!
    return false;
}


}

