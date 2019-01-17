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
    #define NUCLEOBOARD
    #ifdef NUCLEOBOARD

    public:

        /** \brief Constructor */
        SpiChipSelectDriver(IOutputPin& cs_pin_0, IOutputPin& cs_pin_1, IOutputPin& cs_pin_2)
        : m_cs_pins()
        {
            m_cs_pins[0u] = &cs_pin_0;
            m_cs_pins[1u] = &cs_pin_1;
            m_cs_pins[2u] = &cs_pin_2;
            m_cs_pins[3u] = &cs_pin_2;
            m_cs_pins[4u] = &cs_pin_2;
        }

        /** \brief Configure the SPI chip select driver */
        virtual bool configure() { m_cs_pins[0u]->setHigh(); m_cs_pins[1u]->setHigh(); m_cs_pins[2u]->setHigh(); return true; }

        /** \brief Enable the selected peripheral */
        virtual void enable(const uint8_t peripheral) { m_cs_pins[peripheral]->setLow(); }

        /** \brief Disable the selected peripheral */
        virtual void disable(const uint8_t peripheral) { m_cs_pins[peripheral]->setHigh(); }


    private:

        /** \brief CS pins */
        IOutputPin* m_cs_pins[5u];

    #else

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

    #endif // NUCLEOBOARD
};

}

#endif // SPICHIPSELECTDRIVER_H
