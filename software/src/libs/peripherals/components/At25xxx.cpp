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


namespace open_vario
{


/** \brief Constructor */
At25xxx::At25xxx(ISpi& spi, const uint8_t chip_select, const uint16_t size)
: m_spi(spi)
, m_chip_select(chip_select)
, m_size(size)
{}

/** \brief Configure the EEPROM */
bool At25xxx::configure()
{
    // Enable write operations
    const uint8_t write_en_cmd = 0x06u;
    ISpi::XFer spi_xfer;
    spi_xfer.write_data = &write_en_cmd;
    spi_xfer.size = sizeof(write_en_cmd);
    spi_xfer.cs = m_chip_select;

    return m_spi.xfer(spi_xfer);
}

/** \brief Read data from the EEPROM */
bool At25xxx::read(const uint16_t address, uint8_t data[], const uint16_t size)
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
        uint8_t read_cmd[] = {0x03u, static_cast<uint8_t>((address >> 8u) & 0x0Fu), static_cast<uint8_t>(address & 0x0Fu)};
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
bool At25xxx::write(const uint16_t address, const uint8_t data[], const uint16_t size)
{
    bool ret = false;

    // Check address
    if (address + size <= m_size)
    {
        uint16_t index = 0u;
        uint16_t left = size;
        ret = true;
        while (ret && (left != 0u))
        {
            // Compute transfer size
            uint8_t xfer_size = 64u;
            if (left < xfer_size)
            {
                xfer_size = left;
            }

            // Prepare SPI transfer for the data to write
            ISpi::XFer spi_xfer_data;
            spi_xfer_data.write_data = &data[index];
            spi_xfer_data.size = xfer_size;
            spi_xfer_data.cs = m_chip_select;

            // Prepare SPI transfer to send the READ command
            uint8_t write_cmd[] = {0x02u, static_cast<uint8_t>(((address + index) >> 8u) & 0x0Fu), static_cast<uint8_t>((address + index) & 0x0Fu)};
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
                do
                {
                    // Read status register
                    uint8_t read_sr[] = {0x05u, 0x00u};
                    spi_xfer_cmd.write_data = read_sr;
                    spi_xfer_cmd.read_data = read_sr;
                    spi_xfer_cmd.size = sizeof(read_sr);
                    spi_xfer_cmd.cs = m_chip_select;
                    spi_xfer_cmd.keep_cs_active = false;
                    spi_xfer_cmd.next = NULL;
                    ret = m_spi.xfer(spi_xfer_cmd);
                    if (ret)
                    {
                        ready = ((read_sr[1u] & (1u << 0u)) == 0u);
                    }
                }
                while (ret && ready);

                // Next transfer
                index += xfer_size;
                left -= xfer_size;
            }
        }
    }

    return ret;
}


}
