/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_DATA_H
#define OV_DATA_H

#include "i_accelerometer_sensor.h"
#include "i_barometric_altimeter.h"
#include "i_gnss.h"

namespace ov
{

/** @brief Open Vario sensor data */
struct ov_data
{
    /** @brief GNSS data */
    i_gnss::data gnss;
    /** @brief Barometric altimeter data */
    i_barometric_altimeter::data altimeter;
    /** @brief Accelerometer data */
    i_accelerometer_sensor::data accelerometer;
    /** @brief Sink rate (1 = 0.1m/s) */
    int16_t sink_rate;
    /** @brief Glide ratio (1 = 0.1) */
    uint16_t glide_ratio;

    /** @brief Invalid glide ratio value */
    static constexpr uint16_t INVALID_GLIDE_RATIO_VALUE = 9999u;
};

namespace data
{

// Getters

/** @brief Get the sensor data */
ov_data get();

/** @brief Get the GNSS data */
i_gnss::data get_gnss();

/** @brief Get the barometric altimeter data */
i_barometric_altimeter::data get_altimeter();

/** @brief Get the accelerometer data */
i_accelerometer_sensor::data get_accelerometer();

/** @brief Get the sink rate */
int16_t get_sink_rate();

/** @brief Get the glide ratio */
uint16_t get_glide_ratio();

// Setters

/** @brief Set the GNSS data */
void set_gnss(const i_gnss::data& data);

/** @brief Invalidate GNSS data */
void invalidate_gnss();

/** @brief Set the barometric altimeter data */
void set_altimeter(const i_barometric_altimeter::data& data);

/** @brief Set the accelerometer data */
void set_accelerometer(const i_accelerometer_sensor::data& data);

/** @brief Set the sink rate */
void set_sink_rate(int16_t data);

/** @brief Set the sink rate */
void set_glide_ratio(uint16_t data);

} // namespace data
} // namespace ov

#endif // OV_DATA_H
