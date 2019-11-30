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

#include "SpiChipSelectDriver.h"

#include <cstddef>

namespace open_vario
{


/** \brief Constructor */
SpiChipSelectDriver::SpiChipSelectDriver(Stm32l476Gpio& cs0, Stm32l476Gpio& cs1, Stm32l476Gpio& cs2)
: m_cs_lines()
{
    m_cs_lines[0] = &cs0;
    m_cs_lines[1] = &cs1;
    m_cs_lines[2] = &cs2;
}

/** \brief Configure the SPI chip select driver */
bool SpiChipSelectDriver::configure()
{
    for (uint32_t i = 0; i < CS_LINES_COUNT; i++)
    {
        disable(i);
    }
    return true;
}

/** \brief Enable the selected peripheral */
void SpiChipSelectDriver::enable(const uint8_t peripheral)
{ 
    m_cs_lines[peripheral]->setLow();
}

/** \brief Disable the selected peripheral */
void SpiChipSelectDriver::disable(const uint8_t peripheral)
{
    m_cs_lines[peripheral]->setHigh();
}


}
