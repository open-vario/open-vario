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

        /** \brief Number of chip select lines */
        static const uint32_t CS_LINES_COUNT = 3u;

        /** \brief Constructor */
        SpiChipSelectDriver(Stm32l476Gpio& cs0, Stm32l476Gpio& cs1, Stm32l476Gpio& cs2);


        /** \brief Configure the SPI chip select driver */
        virtual bool configure();

        /** \brief Enable the selected peripheral */
        virtual void enable(const uint8_t peripheral);

        /** \brief Disable the selected peripheral */
        virtual void disable(const uint8_t peripheral);


    private:

        /** \brief Chip select lines */
        Stm32l476Gpio* m_cs_lines[CS_LINES_COUNT];
};

}

#endif // SPICHIPSELECTDRIVER_H
