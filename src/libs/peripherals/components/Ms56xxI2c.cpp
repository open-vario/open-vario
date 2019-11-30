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

#include "Ms56xxI2c.h"
#include "II2c.h"
#include "IOs.h"

namespace open_vario
{


/** \brief Constructor */
Ms56xxI2c::Ms56xxI2c(II2c& i2c, const uint8_t address)
: m_i2c(i2c)
, m_address(address)
{}


/** \brief Reset the chip */
bool Ms56xxI2c::reset()
{
    uint8_t reset_cmd = Ms56xx::RESET_CMD;
    II2c::XFer i2c_xfer;
    i2c_xfer.read = false;
    i2c_xfer.data = &reset_cmd;
    i2c_xfer.size = sizeof(reset_cmd);

    const bool ret = m_i2c.xfer(m_address, i2c_xfer);
    if (ret)
    {
        // Wait end of reset (~2.8ms)
        IOs::getInstance().waitMs(20u);
    }

    return ret;
}

/** \brief Read calibration data from the PROM */
bool Ms56xxI2c::readCalibrationData(CalibrationData& calib_data)
{
    bool ret = true;
    uint16_t* const prom = reinterpret_cast<uint16_t*>(&calib_data);

    // Read all the prom
    for (uint8_t i = 0; i < (sizeof(CalibrationData) / sizeof(uint16_t)); i++)
    {
        // Calibration data
        II2c::XFer i2c_xfer_data;
        uint8_t read_data[2u];
        i2c_xfer_data.data = read_data;
        i2c_xfer_data.size = sizeof(uint16_t);

        // Command
        uint8_t read_calib_cmd = Ms56xx::PROM_READ + (i << 1);
        II2c::XFer i2c_xfer_cmd;
        i2c_xfer_cmd.read = false;
        i2c_xfer_cmd.data = &read_calib_cmd;
        i2c_xfer_cmd.size = sizeof(read_calib_cmd);
        i2c_xfer_cmd.next = &i2c_xfer_data;

        ret = ret && m_i2c.xfer(m_address, i2c_xfer_cmd);
        if (ret)
        {
            // Decode received data (MSB is received first)
            prom[i] = (read_data[0u] << 8u) + read_data[1u];
        }
    }

    return ret;
}

/** \brief Read digital pressure value */
bool Ms56xxI2c::readD1(uint32_t& d1)
{
    return readConvertedValue(Ms56xx::CONVERT_D1, d1);
}

/** \brief Read temperature value */
bool Ms56xxI2c::readD2(uint32_t& d2)
{
    return readConvertedValue(Ms56xx::CONVERT_D2, d2);
}

/** \brief Read a converted value */
bool Ms56xxI2c::readConvertedValue(uint8_t cmd, uint32_t& value)
{
    bool ret;

    // Start conversion
    II2c::XFer i2c_xfer;
    i2c_xfer.read = false;
    i2c_xfer.data = &cmd;
    i2c_xfer.size = sizeof(cmd);
    ret = m_i2c.xfer(m_address, i2c_xfer);
    if (ret)
    {
        // Wait end of conversion (~8.22ms)
        IOs::getInstance().waitMs(20u);

        // Adc value
        II2c::XFer i2c_xfer_data;
        uint8_t read_data[3u];
        i2c_xfer_data.data = read_data;
        i2c_xfer_data.size = sizeof(read_data);

        // Command
        uint8_t read_cmd = Ms56xx::ADC_READ;
        i2c_xfer.read = false;
        i2c_xfer.data = &read_cmd;
        i2c_xfer.size = sizeof(read_cmd);
        i2c_xfer.next = &i2c_xfer_data;

        ret = m_i2c.xfer(m_address, i2c_xfer);
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
