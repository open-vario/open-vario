

#include "ms56xx_i2c.h"
#include "thread.h"

namespace ov
{

/** @brief Constructor */
ms56xx_i2c::ms56xx_i2c(i_i2c& i2c, uint8_t address) : m_i2c(i2c), m_address(address) { }

/** @brief Reset the chip */
bool ms56xx_i2c::reset()
{
    // According to datasheet, the chip may be blocked in the acknowledge state
    // The only way to get the MS56xx to function is to send several SCLKs followed by a reset sequence
    bool     ret     = false;
    uint32_t retries = 0;
    do
    {
        uint8_t          reset_cmd = ms56xx::RESET_CMD;
        i_i2c::xfer_desc i2c_xfer;
        i2c_xfer.read = false;
        i2c_xfer.data = &reset_cmd;
        i2c_xfer.size = sizeof(reset_cmd);

        ret = m_i2c.xfer(m_address, i2c_xfer);
        retries++;
    } while (!ret && (retries < 10u));
    if (ret)
    {
        // Wait end of reset (~2.8ms)
        ov::this_thread::sleep_for(10u);
    }

    return ret;
}

/** @brief Read calibration data from the PROM */
bool ms56xx_i2c::read_calibration_data(calibration_data& calib_data)
{
    bool            ret  = true;
    uint16_t* const prom = reinterpret_cast<uint16_t*>(&calib_data);

    // Read all the prom
    for (uint8_t i = 0; i < (sizeof(calibration_data) / sizeof(uint16_t)); i++)
    {
        // Calibration data
        i_i2c::xfer_desc i2c_xfer_data;
        uint8_t          read_data[2u];
        i2c_xfer_data.data = read_data;
        i2c_xfer_data.size = sizeof(uint16_t);

        // Command
        uint8_t          read_calib_cmd = ms56xx::PROM_READ + (i << 1);
        i_i2c::xfer_desc i2c_xfer_cmd;
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

/** @brief Read digital pressure value */
bool ms56xx_i2c::read_d1(uint32_t& d1)
{
    return read_converted_value(ms56xx::CONVERT_D1, d1);
}

/** @brief Read temperature value */
bool ms56xx_i2c::read_d2(uint32_t& d2)
{
    return read_converted_value(ms56xx::CONVERT_D2, d2);
}

/** @brief Read a converted value */
bool ms56xx_i2c::read_converted_value(uint8_t cmd, uint32_t& value)
{
    bool ret;

    // Start conversion
    i_i2c::xfer_desc i2c_xfer;
    i2c_xfer.read = false;
    i2c_xfer.data = &cmd;
    i2c_xfer.size = sizeof(cmd);
    ret           = m_i2c.xfer(m_address, i2c_xfer);
    if (ret)
    {
        // Wait end of conversion (~8.22ms)
        ov::this_thread::sleep_for(10u);

        // Adc value
        i_i2c::xfer_desc i2c_xfer_data;
        uint8_t          read_data[3u];
        i2c_xfer_data.data = read_data;
        i2c_xfer_data.size = sizeof(read_data);

        // Command
        uint8_t read_cmd = ms56xx::ADC_READ;
        i2c_xfer.read    = false;
        i2c_xfer.data    = &read_cmd;
        i2c_xfer.size    = sizeof(read_cmd);
        i2c_xfer.next    = &i2c_xfer_data;

        ret = m_i2c.xfer(m_address, i2c_xfer);
        if (ret)
        {
            // Decode received data (MSB is received first)
            value = (read_data[0u] << 16u) + (read_data[1u] << 8u) + (read_data[2u] << 0u);
        }
    }

    return ret;
}

} // namespace ov
