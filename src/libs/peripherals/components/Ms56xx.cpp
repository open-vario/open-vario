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

#include "Ms56xx.h"


namespace open_vario
{


/** \brief Constructor */
Ms56xx::Ms56xx()
: m_configured(false)
, m_calib_data()
, m_pressure(0u)
, m_temperature(0)
{}

/** \brief Configure the barometric sensor */
bool Ms56xx::configure()
{
    bool ret = true;

    // Check if already configured
    if (!m_configured)
    {
        // Reset the chip
        ret = reset();
        ret = reset();

        // Read the calibration data from the PROM
        ret = ret && readCalibrationData(m_calib_data);

        // Check the CRC in the PROM
        //ret = ret && checkPromCrc4();

        // Chip is now configured
        m_configured = ret;
    }

    return ret;
}

/** \brief Read the pressure (1 = 0.01mbar) */
bool Ms56xx::readPressure(uint32_t& pressure)
{
    bool ret;

    // Read pressure and temperature
    uint32_t D1 = 0u;
    uint32_t D2 = 0u;
    ret = readD1(D1);
    ret = ret && readD2(D2);
    if (ret)
    {
        // Calculate temperature
        const int32_t dT = D2 - m_calib_data.c5 * 256;
        int32_t TEMP = 2000 + (dT * m_calib_data.c6) / 8388608;

        // Calculate temperature compensated offset and sensitivity
        int64_t OFF = m_calib_data.c2 * 131072 + (m_calib_data.c4 * dT) / 64;
        int64_t SENS = m_calib_data.c1 * 65536 + (m_calib_data.c3 * dT) / 128;

        // Second order temperature compensation
        if (TEMP < 2000)
        {
            const int32_t TEMP2000 = (TEMP - 2000) * (TEMP - 2000);
            const int32_t T2 = dT * dT / 2147483648;
            int64_t OFF2 = 61 * TEMP2000 / 16;
            int64_t SENS2 = 2 * TEMP2000;
            if (TEMP < -1500)
            {
                const int32_t TEMP1500 = (TEMP + 1500) * (TEMP + 1500);
                OFF2 += 15 * TEMP1500;
                SENS2 += 8 * TEMP1500;
            }

            // Temperature compensated values
            TEMP -= T2;
            OFF -= OFF2;
            SENS -= SENS2;
        }

        // Calculate temperature compensated pressure
        const int64_t P = ((D1 * SENS) / 2097152 - OFF) / 32768;

        // Save computed values
        m_pressure = static_cast<uint32_t>(P);
        m_temperature = static_cast<int16_t>(TEMP / 10);

        // Return value
        pressure = m_pressure;
    }

    return ret;
}

/** \brief Read the temperature (1 = 0.1°C) */
bool Ms56xx::readTemperature(int16_t& temperature)
{
    // Return previously computed temperature
    temperature = m_temperature;
    return true;
}


/** \brief Check the 4-bit CRC of the calibration data */
bool Ms56xx::checkPromCrc4()
{
    uint16_t crc = 0x00u;
    uint8_t prom_crc = (m_calib_data.crc & 0x000Fu);
    uint16_t* const prom = reinterpret_cast<uint16_t*>(&m_calib_data);

    m_calib_data.crc &= 0xFF00u;
    for (uint8_t cnt = 0u; cnt < 16u; cnt++)
    {
        if ((cnt & 1) == 1)
        {
            crc ^= static_cast<uint16_t>((prom[cnt >> 1u]) & 0x00FFu);
        }
        else 
        {
            crc ^= static_cast<uint16_t>(prom[cnt >> 1u] >> 8u);
        }
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if (crc & (0x8000u))
            {
                crc = (crc << 1u) ^ 0x3000u;
            }
            else
            {
                crc = (crc << 1u);
            }
        }
    }
    crc = (0x000F & (crc >> 12u));

    return (crc == prom_crc);
}


}
