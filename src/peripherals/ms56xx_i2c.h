/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_MS56XX_I2C_H
#define OV_MS56XX_I2C_H

#include "i_i2c.h"
#include "ms56xx.h"

namespace ov
{

/** @brief MS56XX barometric sensor on I2C bus */
class ms56xx_i2c : public ms56xx
{
  public:
    /** @brief Constructor */
    ms56xx_i2c(i_i2c& i2c, uint8_t address);

  protected:
    /** @brief Reset the chip */
    bool reset() override;

    /** @brief Read calibration data from the PROM */
    bool read_calibration_data(calibration_data& calib_data) override;

    /** @brief Read digital pressure value */
    bool read_d1(uint32_t& d1) override;

    /** @brief Read temperature value */
    bool read_d2(uint32_t& d2) override;

  private:
    /** @brief I2C driver */
    i_i2c& m_i2c;
    /** @brief I2C address */
    uint8_t m_address;

    /** @brief Read a converted value */
    bool read_converted_value(uint8_t cmd, uint32_t& value);
};

} // namespace ov

#endif // OV_MS56XX_I2C_H
