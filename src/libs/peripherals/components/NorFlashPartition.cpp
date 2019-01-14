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

#include "NorFlashPartition.h"


namespace open_vario
{


/** \brief Constructor */
NorFlashPartition::NorFlashPartition(INorFlash& nor_flash, const uint32_t first_sector, const uint32_t sector_count)
: m_nor_flash(nor_flash)
, m_first_sector(first_sector)
, m_sector_count(sector_count)
, m_size(m_nor_flash.getSectorSize() * m_sector_count)
, m_start_address(m_nor_flash.getSectorSize() * first_sector)
{}

/** \brief Configure the NOR flash partition */
bool NorFlashPartition::configure() 
{ 
    // Nothing to do
    return true; 
}

/** \brief Read data from the NOR flash partition */
bool NorFlashPartition::read(const uint32_t address, void* const data, const uint32_t size)
{
    bool ret = false;

    // Check address range
    if ((address + size) <= m_size)
    {
        // Read data
        ret = m_nor_flash.read(m_start_address + address, data, size);
    }

    return ret;
}

/** \brief Write data to the NOR flash partition */
bool NorFlashPartition::write(const uint32_t address, const void* const data, const uint32_t size)
{
    bool ret = false;

    // Check address range
    if ((address + size) <= m_size)
    {
        // Write data
        ret = m_nor_flash.write(m_start_address + address, data, size);
    }

    return ret;
}

/** \brief Erase a sector of the NOR flash partition */
bool NorFlashPartition::sectorErase(const uint32_t address)
{
    bool ret = false;

    // Check address range
    if (address < m_size)
    {
        // Erase sector
        ret = m_nor_flash.sectorErase(m_start_address + address);
    }

    return ret;
}

/** \brief Erase the whole the NOR flash partition */
bool NorFlashPartition::fullErase()
{
    // Erase sectors
    const bool ret = m_nor_flash.sectorErase(m_first_sector, m_sector_count);
    return ret;
}


}
