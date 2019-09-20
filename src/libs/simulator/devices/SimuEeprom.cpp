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

#include "SimuEeprom.h"


namespace open_vario
{


/** \brief Contructor */
SimuEeprom::SimuEeprom(const std::string& binfile_path, const uint16_t size)
: m_binfile_path(binfile_path)
, m_size(size)
, m_file()
{}


/** \brief Configure the EEPROM */
bool SimuEeprom::configure()
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

/** \brief Read data from the EEPROM */
bool SimuEeprom::read(const uint16_t address, void* data, const uint16_t size)
{
    bool ret = false;

    // Check address
    if ((static_cast<uint32_t>(address) + static_cast<uint32_t>(size)) < static_cast<uint32_t>(m_size))
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

/** \brief Write data to the EEPROM */
bool SimuEeprom::write(const uint16_t address, const void* data, const uint16_t size)
{
    bool ret = false;

    // Check address
    if ((static_cast<uint32_t>(address) + static_cast<uint32_t>(size)) < static_cast<uint32_t>(m_size))
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

}
