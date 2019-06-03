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

#ifndef SPICHIPSELECTDRIVER_H
#define SPICHIPSELECTDRIVER_H

#include "ISpi.h"
#include "Stm32l476Gpio.h"

namespace open_vario
{

/** \brief SPI chip select driver */
class SpiChipSelectDriver : public ISpiChipSelect
{
    public:

        /** \brief Constructor */
        SpiChipSelectDriver(const Stm32l476Gpio::Port port);


        /** \brief Configure the SPI chip select driver */
        virtual bool configure();

        /** \brief Enable the selected peripheral */
        virtual void enable(const uint8_t peripheral);

        /** \brief Disable the selected peripheral */
        virtual void disable(const uint8_t peripheral);


    private:

        /** \brief GPIO register to driver the chip select lines */
        volatile uint32_t* m_cs_gpio_reg;
};

}

#endif // SPICHIPSELECTDRIVER_H
