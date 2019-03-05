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

#include "Ms56xxSpi.h"
#include "ISpi.h"
#include "IOs.h"

namespace open_vario
{


/** \brief Constructor */
Ms56xxSpi::Ms56xxSpi(ISpi& spi, const uint8_t chip_select)
: m_spi(spi)
, m_chip_select(chip_select)
{}


/** \brief Reset the chip */
bool Ms56xxSpi::reset()
{
    const uint8_t reset_cmd = Ms56xx::RESET_CMD;
    ISpi::XFer spi_xfer;
    spi_xfer.write_data = &reset_cmd;
    spi_xfer.size = sizeof(reset_cmd);
    spi_xfer.cs = m_chip_select;
    spi_xfer.keep_cs_active = true;

    const bool ret = m_spi.xfer(spi_xfer);
    if (ret)
    {
        // Wait end of reset (~2.8ms)
        IOs::getInstance().waitMs(20u);
        m_spi.releaseCs();
    }

    return ret;
}

/** \brief Read calibration data from the PROM */
bool Ms56xxSpi::readCalibrationData(CalibrationData& calib_data)
{
    bool ret = true;
    uint16_t* const prom = reinterpret_cast<uint16_t*>(&calib_data);

    // Read all the prom
    for (uint8_t i = 0; i < (sizeof(CalibrationData) / sizeof(uint16_t)); i++)
    {
        // Calibration data
        ISpi::XFer spi_xfer_data;
        uint8_t read_data[2u];
        spi_xfer_data.read_data = read_data;
        spi_xfer_data.size = sizeof(uint16_t);
        spi_xfer_data.cs = m_chip_select;

        // Command
        const uint8_t read_calib_cmd = Ms56xx::PROM_READ + (i << 1);
        ISpi::XFer spi_xfer_cmd;
        spi_xfer_cmd.write_data = &read_calib_cmd;
        spi_xfer_cmd.size = sizeof(read_calib_cmd);
        spi_xfer_cmd.cs = m_chip_select;
        spi_xfer_cmd.keep_cs_active = true;
        spi_xfer_cmd.next = &spi_xfer_data;

        ret = ret && m_spi.xfer(spi_xfer_cmd);
        if (ret)
        {
            // Decode received data (MSB is received first)
            prom[i] = (read_data[0u] << 8u) + read_data[1u];
        }
    }

    // TODO: Remove when sensor access is fixed
    calib_data.c1 = 46372u;
    calib_data.c2 = 43981u;
    calib_data.c3 = 29059u;
    calib_data.c4 = 27842u;
    calib_data.c5 = 31553u;
    calib_data.c6 = 28165u;
    
    return ret;
}

/** \brief Read digital pressure value */
bool Ms56xxSpi::readD1(uint32_t& d1)
{
    // TODO: Remove when sensor access is fixed
    d1 = 6465444u;
    return true;
    //return readConvertedValue(Ms56xx::CONVERT_D1, d1);
}

/** \brief Read temperature value */
bool Ms56xxSpi::readD2(uint32_t& d2)
{
    // TODO: Remove when sensor access is fixed
    d2 = 8077636u;
    return true;
    //return readConvertedValue(Ms56xx::CONVERT_D2, d2);
}

/** \brief Read a converted value */
bool Ms56xxSpi::readConvertedValue(const uint8_t cmd, uint32_t& value)
{
    bool ret;

    // Start conversion
    ISpi::XFer spi_xfer;
    spi_xfer.write_data = &cmd;
    spi_xfer.size = sizeof(cmd);
    spi_xfer.cs = m_chip_select;
    spi_xfer.keep_cs_active = true;
    ret = m_spi.xfer(spi_xfer);
    if (ret)
    {
        // Wait end of conversion (~8.22ms)
        IOs::getInstance().waitMs(20u);
        m_spi.releaseCs();

        // Adc value
        ISpi::XFer spi_xfer_data;
        uint8_t read_data[3u];
        spi_xfer_data.read_data = read_data;
        spi_xfer_data.size = sizeof(read_data);
        spi_xfer_data.cs = m_chip_select;

        // Command
        const uint8_t read_cmd = Ms56xx::ADC_READ;
        spi_xfer.write_data = &read_cmd;
        spi_xfer.size = sizeof(read_cmd);
        spi_xfer.cs = m_chip_select;
        spi_xfer.keep_cs_active = true;
        spi_xfer.next = &spi_xfer_data;

        ret = m_spi.xfer(spi_xfer);
        if (ret)
        {
            // Decode received data (MSB is received first)
            value = (read_data[0u] << 16u) +
                    (read_data[1u] << 8u) +
                    (read_data[2u] << 0u);
        }
    }

    return ret;
}


}
