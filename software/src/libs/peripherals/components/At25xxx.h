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

#ifndef AT25XXX_H
#define AT25XXX_H

#include "IEeprom.h"

namespace open_vario
{

class ISpi;


/** \brief Base class for AT25 EEPROM implementations */
class At25xxx : public IEeprom
{
    public:

        /** \brief Constructor */
        At25xxx(ISpi& spi, const uint8_t chip_select, const uint16_t size);


        /** \brief Get the size of the EEPROM in bytes */
        virtual uint16_t getSize() const { return m_size; }

        /** \brief Configure the EEPROM */
        virtual bool configure();

        /** \brief Read data from the EEPROM */
        virtual bool read(const uint16_t address, uint8_t data[], const uint16_t size);

        /** \brief Write data to the EEPROM */
        virtual bool write(const uint16_t address, const uint8_t data[], const uint16_t size);


    private:

        /** \brief SPI bus */
        ISpi& m_spi;

        /** \brief Chip select */
        const uint8_t m_chip_select;

        /** \brief EEPROM's size in bytes */
        const uint16_t m_size;
};

}

#endif // AT25XXX_H
