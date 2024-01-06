
#ifndef OV_DATA_H
#define OV_DATA_H

#include "i_gnss.h"

namespace ov
{

/** @brief Open Vario sensor data */
struct ov_data
{
    /** @brief GNSS data */
    i_gnss::data gnss;
};

namespace data
{

// Getters

/** @brief Get the sensor data */
ov_data get();

/** @brief Get the GNSS data */
i_gnss::data get_gnss();

// Setters

/** @brief Set the GNSS data */
void set_gnss(const i_gnss::data& data);

/** @brief Invalidate GNSS data */
void invalidate_gnss();

} // namespace data
} // namespace ov

#endif // OV_DATA_H
