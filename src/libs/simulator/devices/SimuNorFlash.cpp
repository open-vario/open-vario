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

#include "SimuNorFlash.h"


namespace open_vario
{


/** \brief Contructor */
SimuNorFlash::SimuNorFlash(const std::string& binfile_path, const uint32_t size, const uint32_t sector_size)
: m_binfile_path(binfile_path)
, m_size(size)
, m_sector_size(sector_size)
, m_sector_count(m_size / m_sector_size)
, m_file()
, m_erase_buffer()
{
    memset(m_erase_buffer, 0xFF, sizeof(m_erase_buffer));
}


/** \brief Configure the NOR flash */
bool SimuNorFlash::configure()
{
    bool ret;

    // Open the binary file
    ret = m_file.open(m_binfile_path.c_str(), IFile::FA_RW, IFile::FM_CREATE);
    if (ret)
    {
        // Set the binary file size
        ret = m_file.setFileSize(m_size);
    }

    return ret;
}

/** \brief Read data from the NOR flash */
bool SimuNorFlash::read(const uint32_t address, void* const data, const uint32_t size)
{
    bool ret = false;

    // Check address
    if ((address + size) < m_size)
    {
        // Set file pointer
        int32_t new_position = 0;
        ret = m_file.seek(IFile::SO_BEGIN, address, new_position);
        if (ret)
        {
            // Read data
            size_t read = 0u;
            ret = m_file.read(data, size, read);
        }
    }

    return ret;
}

/** \brief Write data to the NOR flash */
bool SimuNorFlash::write(const uint32_t address, const void* const data, const uint32_t size)
{
    bool ret = false;

    // Check address
    if ((address + size) < m_size)
    {
        // Set file pointer
        int32_t new_position = 0;
        ret = m_file.seek(IFile::SO_BEGIN, address, new_position);
        if (ret)
        {
            // Write data
            ret = m_file.write(data, size);
        }
    }

    return ret;
}

/** \brief Erase a sector of the NOR flash */
bool SimuNorFlash::sectorErase(const uint32_t address)
{
    bool ret = false;

    // Check address
    const uint32_t sector_number = address / m_sector_size;
    if (sector_number < m_sector_count)
    {
        // Compute sector start address
        const uint32_t start_address = sector_number * m_sector_size;

        // Set file pointer
        int32_t new_position = 0;
        ret = m_file.seek(IFile::SO_BEGIN, start_address, new_position);
        if (ret)
        {
            // Write data to erase the sector
            uint32_t left = m_sector_size;
            do
            {
                uint32_t write_count = left;
                if (write_count > sizeof(m_erase_buffer))
                {
                    write_count = sizeof(m_erase_buffer);
                }
                left -= write_count;

                ret = m_file.write(m_erase_buffer, write_count);
            }
            while (ret && (left != 0u));
        }
    }

    return ret;
}

/** \brief Erase a range of sectors of the NOR flash */
bool SimuNorFlash::sectorErase(const uint32_t first_sector, const uint32_t sector_count)
{
    bool ret = false;

    // Check sectors
    if (first_sector + sector_count <= m_sector_count)
    {
        // Erase sectors
        ret = true;
        for (uint32_t sector = first_sector; (sector < sector_count) && ret; sector++)
        {
            ret = sectorErase(sector * m_sector_size);
        }
    }

    return ret;
}

/** \brief Erase the whole the NOR flash */
bool SimuNorFlash::chipErase()
{
    return sectorErase(0, m_sector_count);
}

/** \brief Read the NOR flash ID */
bool SimuNorFlash::readId(uint8_t& manufacturer_id, uint8_t& device_type, uint8_t& device_id)
{
    manufacturer_id = 0;
    device_type = 0;
    device_id = 0;
    return true;
}


}
