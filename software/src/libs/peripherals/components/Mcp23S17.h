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

#ifndef MCP23S17_H
#define MCP23S17_H

#include "IIOExpander.h"
#include "ISpi.h"
#include "Mutex.h"

namespace open_vario
{


/** \brief MCP23S17 SPI I/O expander */
class Mcp23S17 : public IIOExpander
{
    public:

        /** \brief Constructor */
        Mcp23S17(ISpi& spi, const uint8_t chip_select);


        /** \brief Configure the I/O expander */
        virtual bool configure();

        /** \brief Configure the mode of an I/O */
        virtual bool configurePin(const uint8_t pin, const bool input);

        /** \brief Set the level of an I/O */
        virtual void setLevel(const uint8_t pin, const IIoPin::Level level);

        /** \brief Get the level of an I/O */
        virtual IIoPin::Level getLevel(const uint8_t pin);


    private:

        /** \brief SPI bus */
        ISpi& m_spi;

        /** \brief Chip select */
        const uint8_t m_chip_select;

        /** \brief Mutex */
        Mutex m_mutex;


        /** \brief I/O direction register A value */
        uint8_t m_iodirA;

        /** \brief I/O direction register B value */
        uint8_t m_iodirB;

        /** \brief I/O output latch register A value */
        uint8_t m_olatA;

        /** \brief I/O output latch register B value */
        uint8_t m_olatB;



        /** \brief Write a value into a register */
        bool writeRegister(const uint8_t reg, const uint8_t value);

        /** \brief Read a value from a register */
        bool readRegister(const uint8_t reg, uint8_t& value);




        /** \brief IODIRA register address */
        static const uint8_t IODIRA = 0x00u;

        /** \brief IODIRB register address */
        static const uint8_t IODIRB = 0x01u;

        /** \brief GPIOA register address */
        static const uint8_t GPIOA = 0x14u;

        /** \brief GPIOB register address */
        static const uint8_t GPIOB = 0x15u;

        /** \brief OLATA register address */
        static const uint8_t OLATA = 0x14u;

        /** \brief OLATB register address */
        static const uint8_t OLATB = 0x15u;

};

}

#endif // MCP23S17_H
