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

#ifndef EEPROMPARTITION_H
#define EEPROMPARTITION_H

#include "IEeprom.h"

namespace open_vario
{



/** \brief EEPROM flash partition */
class EepromPartition
{
    public:

        /** \brief Constructor */
        EepromPartition(IEeprom& eeprom, const uint16_t start_address, const uint16_t size);


        /** \brief Get the size of the EEPROM partition in bytes */
        uint16_t getSize() const { return m_size; }

        /** \brief Configure the EEPROM partition */
        bool configure();

        /** \brief Read data from the EEPROM partition */
        bool read(const uint16_t address, void* data, const uint16_t size);

        /** \brief Write data to the EEPROM partition */
        bool write(const uint16_t address, const void* data, const uint16_t size);


    private:

        /** \brief EEPROM */
        IEeprom& m_eeprom;

        /** \brief Start address of the partition in the EEPROM */
        const uint16_t m_start_address;

        /** \brief Size of the partition in bytes */
        const uint16_t m_size;

};

}

#endif // EEPROMPARTITION_H
