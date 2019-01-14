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

    const bool ret = m_spi.xfer(spi_xfer);
    if (ret)
    {
        // Wait end of reset (~2.8ms)
        IOs::getInstance().waitMs(20u);
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
        spi_xfer_data.read_data = reinterpret_cast<uint8_t*>(&prom[i]);
        spi_xfer_data.size = sizeof(uint16_t);
        spi_xfer_data.cs = m_chip_select;

        // Command
        const uint8_t read_calib_cmd = Ms56xx::PROM_READ + (i * 2u);
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
            const uint16_t rx_value = (spi_xfer_data.read_data[0u] << 8u) + spi_xfer_data.read_data[1u];
            prom[i] = rx_value;
        }
    }

    return ret;
}

/** \brief Read digital pressure value */
bool Ms56xxSpi::readD1(uint32_t& d1)
{
    return readConvertedValue(Ms56xx::CONVERT_D1, d1);
}

/** \brief Read temperature value */
bool Ms56xxSpi::readD2(uint32_t& d2)
{
    return readConvertedValue(Ms56xx::CONVERT_D2, d2);
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
    ret = m_spi.xfer(spi_xfer);
    if (ret)
    {
        // Wait end of conversion (~2.1ms)
        IOs::getInstance().waitMs(20u);

        // Adc value
        ISpi::XFer spi_xfer_data;
        spi_xfer_data.read_data = reinterpret_cast<uint8_t*>(&value);
        spi_xfer_data.size = sizeof(value);
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
            const uint32_t rx_value = (spi_xfer_data.read_data[0u] << 24u) +
                                      (spi_xfer_data.read_data[1u] << 16u) +
                                      (spi_xfer_data.read_data[2u] << 8u) + 
                                      spi_xfer_data.read_data[3u];
            value = rx_value;
        }
    }

    return ret;
}


}
