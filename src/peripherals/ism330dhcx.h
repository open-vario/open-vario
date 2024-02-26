/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_ISM330DHCX_H
#define OV_ISM330DHCX_H

#include "i_accelerometer_sensor.h"
#include "i_i2c.h"

namespace ov
{

/** @brief Interface for accelerometer sensor implementations */
class ism330dhcx : public i_accelerometer_sensor
{
  public:
    /** @brief Constructor */
    ism330dhcx(i_i2c& i2c, uint8_t address);

    /** @brief Initialize the barometric sensor */
    bool init();

    /** @brief Get the accelerometer sensor data */
    data get_data() override;

  private:
    /** @brief Device identifier */
    static constexpr uint8_t DEVICE_ID = 0x6Bu;

    /** @brief I2C driver */
    i_i2c& m_i2c;
    /** @brief I2C address */
    uint8_t m_address;

    /** @brief Reset the sensor */
    bool reset();
    /** @brief Set the sensor in configuration mode */
    bool set_config_mode(bool is_enabled);
    /** @brief Set the sensor in auto increment mode */
    bool set_auto_inc_mode(bool is_enabled);
    /** @brief Set the sensor in block data update mode */
    bool set_bdu_mode(bool is_enabled);
    /** @brief Set the FIFO mode of the sensor */
    bool set_fifo_mode(uint8_t mode);
    /** @brief Set the datarate and the scale of the sensor */
    bool set_data_rate_scale(uint8_t datarate, uint8_t scale);

    /** @brief Write a bit in a register */
    bool write_bit(uint8_t reg, uint8_t bit, bool value);
    /** @brief Read a bit from a register */
    bool read_bit(uint8_t reg, uint8_t bit, bool& value);

    /** @brief Write data to a register */
    bool write_reg(uint8_t reg, uint8_t value);
    /** @brief Read data from a register */
    bool read_reg(uint8_t reg, uint8_t& value);
};

} // namespace ov

#endif // OV_ISM330DHCX_H
