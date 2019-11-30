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

#include "EepromPartition.h"


namespace open_vario
{


/** \brief Constructor */
EepromPartition::EepromPartition(IEeprom& eeprom, const uint16_t start_address, const uint16_t size)
: m_eeprom(eeprom)
, m_start_address(start_address)
, m_size(size)
{}

/** \brief Configure the EEPROM partition */
bool EepromPartition::configure()
{
    // Nothing to do
    return true; 
}

/** \brief Read data from the EEPROM partition */
bool EepromPartition::read(const uint16_t address, void* data, const uint16_t size)
{
    bool ret = false;

    // Check address range
    if ((address + size) <= m_size)
    {
        // Read data
        ret = m_eeprom.read(m_start_address + address, data, size);
    }

    return ret;
}

/** \brief Write data to the EEPROM partition */
bool EepromPartition::write(const uint16_t address, const void* data, const uint16_t size)
{
    bool ret = false;

    // Check address range
    if ((address + size) <= m_size)
    {
        // Write data
        ret = m_eeprom.write(m_start_address + address, data, size);
    }

    return ret;
}


}
