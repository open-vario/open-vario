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

#include "stm32l476xx.h"

#include <cstddef>

namespace open_vario
{

#ifndef NUCLEOBOARD

/** \brief Constructor */
SpiChipSelectDriver::SpiChipSelectDriver(const Stm32l476Gpio::Port port)
: m_cs_gpio_reg(nullptr)
{
    GPIO_TypeDef* const gpio_regs[] = { GPIOA, GPIOB, GPIOC, GPIOD };
    if (port < Stm32l476Gpio::PORT_MAX)
    {
        m_cs_gpio_reg = &gpio_regs[port]->BSRR;
    }
}

/** \brief Configure the SPI chip select driver */
bool SpiChipSelectDriver::configure()
{
    (*m_cs_gpio_reg) = (7u << 16u);
    return true;
}

/** \brief Enable the selected peripheral */
void SpiChipSelectDriver::enable(const uint8_t peripheral)
{ 
    (*m_cs_gpio_reg) = (peripheral & 7u);
}

/** \brief Disable the selected peripheral */
void SpiChipSelectDriver::disable(const uint8_t peripheral)
{
    (*m_cs_gpio_reg) = (7u << 16u);
}

#endif // NUCLEOBOARD

}
