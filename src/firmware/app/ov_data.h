
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

// Setters

/** @brief Set the GNSS data */
void set_gnss(const i_gnss::data& data);

/** @brief Invalidate GNSS data */
void invalidate_gnss();

/** @brief Set the barometric altimeter data */
void set_altimeter(const i_barometric_altimeter::data& data);

/** @brief Set the accelerometer data */
void set_accelerometer(const i_accelerometer_sensor::data& data);

} // namespace data
} // namespace ov

#endif // OV_DATA_H
