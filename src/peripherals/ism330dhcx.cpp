/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "ism330dhcx.h"

#include <cmath>

namespace ov
{

/** @brief FIFO_CTRL4 register address */
static const uint8_t FIFO_CTRL4 = 0x0Au;
/** @brief WHO_AM_I_REG register address */
static const uint8_t WHO_AM_I_REG = 0x0Fu;
/** @brief CTRL3_C register address */
static const uint8_t CTRL3_C_REG = 0x12u;
/** @brief CTRL1_XL register address */
static const uint8_t CTRL1_XL_REG = 0x10u;
/** @brief CTRL9_XL register address */
static const uint8_t CTRL9_XL_REG = 0x18u;
/** @brief OUTX_L_A register address */
static const uint8_t OUTX_L_A_REG = 0x28u;

/** @brief FIFO mode bits in FIFO_CTRL4 register */
static const uint8_t FIFO_CTRL4_FIFO_MODE_BITS = static_cast<uint8_t>(0x7 << 0);

/** @brief SW_RESET bit in CTRL3_C register */
static const uint8_t CTRL3_C_SW_RESET_BIT = static_cast<uint8_t>(1 << 0);
/** @brief IF_INC bit in CTRL3_C register */
static const uint8_t CTRL3_C_IF_INC_BIT = static_cast<uint8_t>(1 << 2);
/** @brief BDU bit in CTRL3_C register */
static const uint8_t CTRL3_C_BDU_BIT = static_cast<uint8_t>(1 << 6);

/** @brief DEVICE_CONF bit in CTRL9_XL_REG register */
static const uint8_t CTRL9_XL_REG_DEVICE_CONF_BIT = static_cast<uint8_t>(1 << 0);

/** @brief Constructor */
ism330dhcx::ism330dhcx(i_i2c& i2c, uint8_t address) : m_i2c(i2c), m_address(address) { }

/** @brief Initialize the barometric sensor */
bool ism330dhcx::init()
{
    bool ret;

    // Check id
    uint8_t device_id = 0;
    ret               = read_reg(WHO_AM_I_REG, device_id);
    ret               = ret && (device_id == DEVICE_ID);

    // Enter configuration mode
    ret = ret && set_config_mode(true);

    // Reset device
    ret = ret && reset();

    // Re-enter configuration mode
    ret = ret && set_config_mode(true);

    // Set address auto-increment mode
    ret = ret && set_auto_inc_mode(true);

    // Set block data update mode
    ret = ret && set_bdu_mode(true);

    // Set FIFO mode to BYPASS = disabled */
    ret = ret && set_fifo_mode(0u);

    // Select output data rate (104Hz) and scale (8g)
    ret = ret && set_data_rate_scale(0x04u, 0x03u);

    // Exit configuration mode
    ret = ret && set_config_mode(false);

    return ret;
}

/** @brief Get the accelerometer sensor data */
i_accelerometer_sensor::data ism330dhcx::get_data()
{
    data sensor_data;

    // Get raw values
    uint8_t          reg = OUTX_L_A_REG;
    i_i2c::xfer_desc i2c_xfer_values;
    i2c_xfer_values.data = reinterpret_cast<uint8_t*>(&sensor_data.x_accel);
    i2c_xfer_values.size = 6u;

    i_i2c::xfer_desc i2c_xfer_cmd;
    i2c_xfer_cmd.read      = false;
    i2c_xfer_cmd.data      = &reg;
    i2c_xfer_cmd.size      = sizeof(reg);
    i2c_xfer_cmd.stop_cond = false;
    i2c_xfer_cmd.next      = &i2c_xfer_values;

    sensor_data.is_valid = m_i2c.xfer(m_address, i2c_xfer_cmd);
    if (sensor_data.is_valid)
    {
        // Apply sensitivity => 0.244f for 8g
        sensor_data.x_accel = static_cast<uint16_t>(static_cast<float>(sensor_data.x_accel) * 0.244f);
        sensor_data.y_accel = static_cast<uint16_t>(static_cast<float>(sensor_data.y_accel) * 0.244f);
        sensor_data.z_accel = static_cast<uint16_t>(static_cast<float>(sensor_data.z_accel) * 0.244f);

        // Compute total acceleration
        sensor_data.total_accel =
            static_cast<uint16_t>(sqrt(static_cast<float>(sensor_data.x_accel) * static_cast<float>(sensor_data.x_accel) +
                                       static_cast<float>(sensor_data.y_accel) * static_cast<float>(sensor_data.y_accel) +
                                       static_cast<float>(sensor_data.z_accel) * static_cast<float>(sensor_data.z_accel)));
    }

    return sensor_data;
}

/** @brief Reset the sensor */
bool ism330dhcx::reset()
{
    return write_bit(CTRL3_C_REG, CTRL3_C_SW_RESET_BIT, true);
}

/** @brief Set the sensor in configuration mode */
bool ism330dhcx::set_config_mode(bool is_enabled)
{
    return write_bit(CTRL9_XL_REG, CTRL9_XL_REG_DEVICE_CONF_BIT, is_enabled);
}

/** @brief Set the sensor in auto increment mode */
bool ism330dhcx::set_auto_inc_mode(bool is_enabled)
{
    return write_bit(CTRL3_C_REG, CTRL3_C_IF_INC_BIT, true);
}

/** @brief Set the sensor in block data update mode */
bool ism330dhcx::set_bdu_mode(bool is_enabled)
{
    return write_bit(CTRL3_C_REG, CTRL3_C_BDU_BIT, true);
}

/** @brief Set the FIFO mode of the sensor */
bool ism330dhcx::set_fifo_mode(uint8_t mode)
{
    uint8_t reg_value = 0u;
    bool    ret       = read_reg(FIFO_CTRL4, reg_value);
    if (ret)
    {
        reg_value &= ~FIFO_CTRL4_FIFO_MODE_BITS;
        reg_value |= mode;
        ret = write_reg(FIFO_CTRL4, reg_value);
    }
    return ret;
}

/** @brief Set the datarate and the scale of the sensor */
bool ism330dhcx::set_data_rate_scale(uint8_t datarate, uint8_t scale)
{
    uint8_t reg_value = (scale << 2) | (datarate << 4);
    return write_reg(CTRL1_XL_REG, reg_value);
}

/** @brief Write a bit in a register */
bool ism330dhcx::write_bit(uint8_t reg, uint8_t bit, bool value)
{
    uint8_t reg_value = 0u;
    bool    ret       = read_reg(reg, reg_value);
    if (ret)
    {
        if (value)
        {
            reg_value |= bit;
        }
        else
        {
            reg_value &= ~bit;
        }
        ret = write_reg(reg, reg_value);
    }
    return ret;
}

/** @brief Read a bit from a register */
bool ism330dhcx::read_bit(uint8_t reg, uint8_t bit, bool& value)
{
    uint8_t reg_value = 0;
    bool    ret       = read_reg(reg, reg_value);
    if (ret)
    {
        value = ((reg_value & bit) != 0u);
    }
    return ret;
}

/** @brief Write data to a register */
bool ism330dhcx::write_reg(uint8_t reg, uint8_t value)
{
    uint8_t cmd[] = {reg, value};

    i_i2c::xfer_desc i2c_xfer_cmd;
    i2c_xfer_cmd.read = false;
    i2c_xfer_cmd.data = cmd;
    i2c_xfer_cmd.size = sizeof(cmd);

    return m_i2c.xfer(m_address, i2c_xfer_cmd);
}

/** @brief Read data from a register */
bool ism330dhcx::read_reg(uint8_t reg, uint8_t& value)
{
    i_i2c::xfer_desc i2c_xfer_value;
    i2c_xfer_value.data = &value;
    i2c_xfer_value.size = sizeof(value);

    i_i2c::xfer_desc i2c_xfer_cmd;
    i2c_xfer_cmd.read      = false;
    i2c_xfer_cmd.data      = &reg;
    i2c_xfer_cmd.size      = sizeof(reg);
    i2c_xfer_cmd.stop_cond = false;
    i2c_xfer_cmd.next      = &i2c_xfer_value;

    return m_i2c.xfer(m_address, i2c_xfer_cmd);
}

} // namespace ov
