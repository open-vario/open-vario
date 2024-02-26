/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_GNSS_H
#define OV_I_GNSS_H

#include "date_time.h"

#include <cstdint>

namespace ov
{

/** @brief Interface for global navigation satellite systems implementations */
class i_gnss
{
  public:
    /** @brief GNSS navigation data */
    struct data
    {
        /** @brief Date and time (UTC) */
        date_time date;
        /** @brief Latitude (1 = 1°) */
        double latitude;
        /** @brief Longitude (1 = 1°) */
        double longitude;
        /** @brief Speed (1 = 0.1 m/s) */
        uint32_t speed;
        /** @brief Altitude (1 = 0.1 m) */
        uint32_t altitude;
        /** @brief Track angle (1 = 0.1°) */
        uint16_t track_angle;
        /** @brief Number of satellites */
        uint8_t satellite_count;
        /** @brief Indicate if the data is valid */
        bool is_valid;
    };

    /** @brief Destructor */
    virtual ~i_gnss() { }

    /** @brief Update navigation data */
    virtual bool update_data() = 0;

    /** @brief Get the current navigation data */
    virtual data get_data() = 0;

    /** @brief Convert a coordinate from decimal degrees (DD) to degrees minutes seconds (DMS) representation */
    static void to_dms(double dd, uint32_t& degrees, uint32_t& minutes, double& seconds);
    /** @brief Convert a coordinate from decimal degrees (DD) to degrees minutes seconds (DMS) representation */
    static void to_dms(double dd, uint32_t& degrees, uint32_t& minutes, uint32_t& seconds);
    /** @brief Convert a coordinate from degrees minutes seconds (DMS) to decimal degrees (DD) representation */
    static void to_dd(uint32_t degrees, uint32_t minutes, double seconds, double& dd);
    /** @brief Convert a coordinate from degrees minutes seconds (DMS) to decimal degrees (DD) representation */
    static void to_dd(uint32_t degrees, uint32_t minutes, uint32_t seconds, double& dd);
};

} // namespace ov

#endif // OV_I_GNSS_H
