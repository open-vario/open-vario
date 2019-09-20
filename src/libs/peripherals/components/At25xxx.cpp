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

#include "At25xxx.h"
#include "ISpi.h"
#include "IOs.h"


namespace open_vario
{


/** \brief Constructor */
At25xxx::At25xxx(ISpi& spi, const uint8_t chip_select, const uint16_t size, const uint16_t page_size)
: m_spi(spi)
, m_chip_select(chip_select)
, m_size(size)
, m_page_size(page_size)
{}

/** \brief Configure the EEPROM */
bool At25xxx::configure()
{
    // Nothing to do
    return true;
}

/** \brief Read data from the EEPROM */
bool At25xxx::read(const uint16_t address, void* data, const uint16_t size)
{
    bool ret = false;

    // Check address
    if (address + size <= m_size)
    {
        // Prepare SPI transfer for the read data
        ISpi::XFer spi_xfer_data;
        spi_xfer_data.read_data = reinterpret_cast<uint8_t*>(data);
        spi_xfer_data.size = size;
        spi_xfer_data.cs = m_chip_select;

        // Prepare SPI transfer to send the READ command
        uint8_t read_cmd[] = {
                                0x03u, 
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

/** \brief Write data to the EEPROM */
bool At25xxx::write(const uint16_t address, const void* data, const uint16_t size)
{
    bool ret = false;

    // Check address
    if (address + size <= m_size)
    {
        uint16_t index = 0u;
        uint16_t left = size;
        uint32_t current_address = address;
        const uint8_t* pdata = reinterpret_cast<const uint8_t*>(data);
        ret = true;
        while (ret && (left != 0u))
        {
            // Enable write operations
            sendWriteEnableCmd();

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

            // Prepare SPI transfer for the data to write
            ISpi::XFer spi_xfer_data;
            spi_xfer_data.write_data = &pdata[index];
            spi_xfer_data.size = program_size;
            spi_xfer_data.cs = m_chip_select;

            // Prepare SPI transfer to send the WRITE command
            uint8_t write_cmd[] = {
                                    0x02u, 
                                    static_cast<uint8_t>(((address + index) >> 8u) & 0xFFu), 
                                    static_cast<uint8_t>((address + index) & 0xFFu)
                                  };
            ISpi::XFer spi_xfer_cmd;
            spi_xfer_cmd.write_data = write_cmd;
            spi_xfer_cmd.size = sizeof(write_cmd);
            spi_xfer_cmd.cs = m_chip_select;
            spi_xfer_cmd.keep_cs_active = true;
            spi_xfer_cmd.next = &spi_xfer_data;

            // Start transfer
            ret = m_spi.xfer(spi_xfer_cmd);
            if (ret)
            {
                // Wait for end of write
                bool ready = false;
                const uint32_t timeout = IOs::getInstance().getMsTimestamp() + WRITE_TIMEOUT;
                do
                {
                    // Read status register
                    uint8_t read_sr[] = {0x05u, 0x00u};
                    spi_xfer_cmd.write_data = read_sr;
                    spi_xfer_cmd.read_data = read_sr;
                    spi_xfer_cmd.size = sizeof(read_sr);
                    spi_xfer_cmd.cs = m_chip_select;
                    spi_xfer_cmd.keep_cs_active = false;
                    spi_xfer_cmd.next = nullptr;
                    ret = m_spi.xfer(spi_xfer_cmd);
                    if (ret)
                    {
                        ready = ((read_sr[1u] & (1u << 0u)) == 0u);
                    }
                }
                while (ret && !ready && (IOs::getInstance().getMsTimestamp() < timeout));

                // Next transfer
                index += program_size;
                left -= program_size;
                current_address += program_size;
            }
        }
    }

    return ret;
}

/** \brief Send the write enable command */
bool At25xxx::sendWriteEnableCmd()
{
    const uint8_t write_en_cmd = 0x06u;
    ISpi::XFer spi_xfer;
    spi_xfer.write_data = &write_en_cmd;
    spi_xfer.size = sizeof(write_en_cmd);
    spi_xfer.cs = m_chip_select;

    return m_spi.xfer(spi_xfer);
}


}
