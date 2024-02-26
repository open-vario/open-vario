/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_MS56XX_H
#define OV_MS56XX_H

#include "i_barometric_sensor.h"

namespace ov
{

/** @brief Base class for MS56XX barometric sensor implementations */
class ms56xx : public i_barometric_sensor
{
  public:
    /** @brief Constructor */
    ms56xx();

    /** @brief Initialize the barometric sensor */
    bool init();

    /** @brief Get the barometric sensor data */
    data get_data() override;

  protected:
    /** @brief Calibration data */
    struct calibration_data
    {
        /** @brief Reserved */
        uint16_t reserved;
        /** @brief Pressure sensitivity | SENSt1*/
        uint16_t c1;
        /** @brief Pressure offset | OFFt1 */
        uint16_t c2;
        /** @brief Temperature coefficient of pressure sensitivity | TCS */
        uint16_t c3;
        /** @brief Temperature coefficient of pressure offset | TCO */
        uint16_t c4;
        /** @brief Reference temperature | Tref */
        uint16_t c5;
        /** @brief Temperature coefficient of the temperature | TSENSE */
        uint16_t c6;
        /** @brief 4-bit CRC */
        uint16_t crc;
    };

    /** @brief Reset the chip */
    virtual bool reset() = 0;

    /** @brief Read calibration data from the PROM */
    virtual bool read_calibration_data(calibration_data& calib_data) = 0;

    /** @brief Read digital pressure value */
    virtual bool read_d1(uint32_t& d1) = 0;

    /** @brief Read temperature value */
    virtual bool read_d2(uint32_t& d2) = 0;

    /** @brief Reset command */
    static const uint8_t RESET_CMD = 0x1Eu;
    /** @brief Convert D1 command */
    static const uint8_t CONVERT_D1 = 0x48u;
    /** @brief Convert D2 command */
    static const uint8_t CONVERT_D2 = 0x58u;
    /** @brief ADC read command */
    static const uint8_t ADC_READ = 0x00u;
    /** @brief PROM read command */
    static const uint8_t PROM_READ = 0xA0u;

  private:
    /** @brief Indicate the chip has been configured */
    bool m_configured;
    /** @brief Calibration data */
    calibration_data m_calib_data;
    /** @brief Sensor data */
    data m_data;

    /** @brief Check the 4-bit CRC of the calibration data */
    bool check_prom_crc4();
};

} // namespace ov

#endif // OV_MS56XX_H
