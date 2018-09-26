
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

#include "SSt26xxx.h"
#include "ISpi.h"


namespace open_vario
{


/** \brief Constructor */
SSt26xxx::SSt26xxx(ISpi& spi, const uint8_t chip_select, const uint32_t size, const uint32_t sector_size, const uint32_t page_size)
: m_spi(spi)
, m_chip_select(chip_select)
, m_size(size)
, m_sector_size(sector_size)
, m_page_size(page_size)
, m_sector_count(m_size / m_sector_size)
{}

/** \brief Configure the NOR flash */
bool SSt26xxx::configure()
{
    bool ret = true;

    // Reset the NOR flash
    ret = ret && sendCommand(RSTEN);
    ret = ret && sendCommand(RST);

    // Wait ready
    ret = ret && waitReady();

    return ret;
}

/** \brief Read data from the NOR flash */
bool SSt26xxx::read(const uint32_t address, uint8_t data[], const uint32_t size)
{
    bool ret = false;

    // Check address
    if (address + size <= m_size)
    {
        // Prepare SPI transfer for the read data
        ISpi::XFer spi_xfer_data;
        spi_xfer_data.read_data = data;
        spi_xfer_data.size = size;
        spi_xfer_data.cs = m_chip_select;

        // Prepare SPI transfer to send the READ command
        uint8_t read_cmd[] = {
                                READ,
                                static_cast<uint8_t>((address >> 16u) & 0xFFu),
                                static_cast<uint8_t>((address >> 8u) & 0xFFu),
                                static_cast<uint8_t>(address & 0xFFu)
                             };
        ISpi::XFer spi_xfer_cmd;
        spi_xfer_cmd.write_data = read_cmd;
        spi_xfer_cmd.size = sizeof(read_cmd);
        spi_xfer_cmd.cs = m_chip_select;
        spi_xfer_cmd.keep_cs_active = true;
        spi_xfer_cmd.next = &spi_xfer_data;

        ret = m_spi.xfer(spi_xfer_cmd);
    }

    return ret;
}

/** \brief Write data to the NOR flash */
bool SSt26xxx::write(const uint32_t address, const uint8_t data[], const uint32_t size)
{
    bool ret = false;

    // Check address
    if (address + size <= m_size)
    {
        // Enable write operations
        ret = sendCommand(WREN);

        ISpi::XFer spi_xfer_data;
        ISpi::XFer spi_xfer_cmd;
        uint16_t index = 0u;
        uint16_t left = size;
        uint32_t current_address = address;
        while (ret && (left != 0u))
        {
            // Check programming page alignment
            const uint32_t page_align = current_address & (m_page_size - 1u);

            // Compute programming size
            uint32_t program_size = left;
            if (page_align != 0u)
            {
                if (program_size > (m_page_size - page_align))
                {
                    program_size = m_page_size - page_align;
                }
            }
            else
            {
                if (program_size > m_page_size)
                {
                    program_size = m_page_size;
                }
            }

            // Program data
            spi_xfer_data.write_data = &data[index];
            spi_xfer_data.size = program_size;
            spi_xfer_data.cs = m_chip_select;

            uint8_t write_cmd[] = {
                                    PP,
                                    static_cast<uint8_t>((current_address >> 16u) & 0xFFu),
                                    static_cast<uint8_t>((current_address >> 8u) & 0xFFu),
                                    static_cast<uint8_t>(current_address & 0xFFu)
                                  };
            spi_xfer_cmd.write_data = write_cmd;
            spi_xfer_cmd.size = sizeof(write_cmd);
            spi_xfer_cmd.cs = m_chip_select;
            spi_xfer_cmd.keep_cs_active = true;
            spi_xfer_cmd.next = &spi_xfer_data;

            ret = m_spi.xfer(spi_xfer_cmd);
            if (ret)
            {
                ret = waitReady();
            }

            // Next data
            index += program_size;
            left -= program_size;
            current_address += program_size;
        }
    }

    return ret;
}

/** \brief Erase a sector of the NOR flash */
bool SSt26xxx::sectorErase(const uint32_t address)
{
    bool ret = false;

    // Check address
    if (address < m_size)
    {
        // Enable write operations
        ret = sendCommand(WREN);

        // Sector erase
        uint8_t params[] = {
                                static_cast<uint8_t>((address >> 16u) & 0xFFu),
                                static_cast<uint8_t>((address >> 8u) & 0xFFu),
                                static_cast<uint8_t>(address & 0xFFu)
                           };
        ret = ret && sendCommand(SE, params, sizeof(params));
    }

    return ret;
}

/** \brief Erase a range of sectors of the NOR flash */
bool SSt26xxx::sectorErase(const uint32_t first_sector, const uint32_t sector_count)
{
    bool ret = true;

    // Check range
    if ((first_sector + sector_count) <= m_sector_count)
    {
        // Erase sectors
        uint32_t sector = first_sector;
        while (ret && (sector < (first_sector + sector_count)))
        {
            ret = sectorErase(sector);
            sector++;
        }
    }

    return ret;
}

/** \brief Erase the whole the NOR flash */
bool SSt26xxx::chipErase()
{
    bool ret;

    // Enable write operations
    ret = sendCommand(WREN);

    // Chip erase
    ret = ret && sendCommand(CE);

    return ret;
}

/** \brief Send a command to the NOR flash */
bool SSt26xxx::sendCommand(const uint8_t cmd)
{
    ISpi::XFer spi_xfer;
    spi_xfer.write_data = &cmd;
    spi_xfer.size = sizeof(cmd);
    spi_xfer.cs = m_chip_select;

    return m_spi.xfer(spi_xfer);
}

/** \brief Send a command with parameters and completion to the NOR flash */
bool SSt26xxx::sendCommand(const uint8_t cmd, const uint8_t params[], const uint32_t length)
{
    ISpi::XFer spi_xfer_params;
    spi_xfer_params.write_data = params;
    spi_xfer_params.size = length;
    spi_xfer_params.cs = m_chip_select;

    ISpi::XFer spi_xfer_cmd;
    spi_xfer_cmd.write_data = &cmd;
    spi_xfer_cmd.size = sizeof(cmd);
    spi_xfer_cmd.cs = m_chip_select;
    spi_xfer_cmd.keep_cs_active = true;
    spi_xfer_cmd.next = &spi_xfer_params;

    bool ret = m_spi.xfer(spi_xfer_cmd);
    if (ret)
    {
        ret = waitReady();
    }

    return ret;
}

/** \brief Wait for the NOR flash to be ready */
bool SSt26xxx::waitReady()
{
    bool ret = false;
    bool ready = false;
    ISpi::XFer spi_xfer;
    do
    {
        // Read status register
        uint8_t read_sr[] = {RDSR, 0x00u};
        spi_xfer.write_data = read_sr;
        spi_xfer.read_data = read_sr;
        spi_xfer.size = sizeof(read_sr);
        spi_xfer.cs = m_chip_select;
        ret = m_spi.xfer(spi_xfer);
        if (ret)
        {
            ready = ((read_sr[1u] & (1u << 0u)) == 0u);
        }
    }
    while (ret && !ready);

    return ready;
}

}
