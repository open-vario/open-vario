/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_BAROMETRIC_ALTIMETER_H
#define OV_I_BAROMETRIC_ALTIMETER_H

#include <cstdint>

namespace ov
{

/** @brief Interface for barometric altimeter implementations */
class i_barometric_altimeter
{
  public:
    /** @brief Barometric altimeter data */
    struct data
    {
        /** @brief Pressure (1 = 0.01mbar) */
        int32_t pressure;
        /** @brief Altitude (1 = 0.1m) */
        int32_t altitude;
        /** @brief Temperature (1 = 0.1°C) */
        int16_t temperature;
        /** @brief Indicate if the data is valid */
        bool is_valid;
    };

    /** @brief Destructor */
    virtual ~i_barometric_altimeter() { }

    /** 
     * @brief Set the references for the altitude computation 
     *        Temperature : 1 = 0.1°C
     *        Pressure : 1 = 0.01mbar
     *        Altitude : 1 = 0.1m
     */
    virtual void set_references(const int16_t ref_temperature, const uint32_t ref_pressure, const int32_t ref_altitude) = 0;

    /** @brief Get the barometric altimeter data */
    virtual data get_data() = 0;
};

} // namespace ov

#endif // OV_I_BAROMETRIC_ALTIMETER_H
