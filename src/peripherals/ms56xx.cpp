
#include "ms56xx.h"

namespace ov
{

/** @brief Constructor */
ms56xx::ms56xx() : m_configured(false), m_calib_data(), m_data{} { }

/** @brief Initialize the barometric sensor */
bool ms56xx::init()
{
    bool ret = true;

    // Check if already configured
    if (!m_configured)
    {
        // Reset the chip
        ret = reset();

        // Read the calibration data from the PROM
        ret = ret && read_calibration_data(m_calib_data);

        // Check the CRC in the PROM
        ret = ret && check_prom_crc4();

        // Chip is now configured
        m_configured = ret;
    }

    return ret;
}

/** @brief Get the barometric sensor data */
i_barometric_sensor::data ms56xx::get_data()
{
    bool is_valid;

    // Read pressure and temperature
    uint32_t D1 = 0u;
    uint32_t D2 = 0u;
    is_valid    = read_d1(D1);
    is_valid    = is_valid && read_d2(D2);
    if (is_valid)
    {
        // Calculate temperature
        const int64_t dT   = static_cast<int32_t>(D2) - static_cast<int32_t>(m_calib_data.c5 * 256);
        int64_t       TEMP = 2000ll + (dT * static_cast<int64_t>(m_calib_data.c6)) / 8388608ll;

        // Calculate temperature compensated offset and sensitivity
        int64_t OFF =
            static_cast<int64_t>(m_calib_data.c2) * 131072ll + (static_cast<int64_t>(m_calib_data.c4) * static_cast<int64_t>(dT)) / 64ll;
        int64_t SENS =
            static_cast<int64_t>(m_calib_data.c1) * 65536ll + (static_cast<int64_t>(m_calib_data.c3) * static_cast<int64_t>(dT)) / 128ll;

        // Second order temperature compensation
        if (TEMP < 2000)
        {
            const int32_t TEMP2000 = (TEMP - 2000) * (TEMP - 2000);
            const int32_t T2       = dT * dT / 2147483648;
            int64_t       OFF2     = 61ll * static_cast<int64_t>(TEMP2000) / 16ll;
            int64_t       SENS2    = 2ll * static_cast<int64_t>(TEMP2000);
            if (TEMP < -1500)
            {
                const int32_t TEMP1500 = (TEMP + 1500) * (TEMP + 1500);
                OFF2 += 15ll * static_cast<int64_t>(TEMP1500);
                SENS2 += 8ll * static_cast<int64_t>(TEMP1500);
            }

            // Temperature compensated values
            TEMP -= T2;
            OFF -= OFF2;
            SENS -= SENS2;
        }

        // Calculate temperature compensated pressure
        const int64_t P = ((static_cast<int64_t>(D1) * SENS) / 2097152ll - OFF) / 32768ll;

        // Save computed values
        m_data.pressure    = static_cast<uint32_t>(P);
        m_data.temperature = static_cast<int16_t>(TEMP / 10);
    }

    // Validity
    m_data.is_valid = is_valid;

    return m_data;
}

/** @brief Check the 4-bit CRC of the calibration data */
bool ms56xx::check_prom_crc4()
{
    uint16_t        crc      = 0x00u;
    uint8_t         prom_crc = (m_calib_data.crc & 0x000Fu);
    uint16_t* const prom     = reinterpret_cast<uint16_t*>(&m_calib_data);

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

} // namespace ov
