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

#ifndef IEEPROM_H
#define IEEPROM_H

#include <cstdint>

namespace open_vario
{



/** \brief Interface for all EEPROMs implementations */
class IEeprom
{
    public:

        /** \brief Get the size of the EEPROM in bytes */
        virtual uint16_t getSize() const = 0;

        /** \brief Configure the EEPROM */
        virtual bool configure() = 0;

        /** \brief Read data from the EEPROM */
        virtual bool read(const uint16_t address, uint8_t data[], const uint16_t size) = 0;

        /** \brief Write data to the EEPROM */
        virtual bool write(const uint16_t address, const uint8_t data[], const uint16_t size) = 0;
};

}

#endif // IEEPROM_H
