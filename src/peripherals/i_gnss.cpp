/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "i_gnss.h"

namespace ov
{

/** @brief Convert a coordinate from decimal degrees (DD) to degrees minutes seconds (DMS) representation */
void i_gnss::to_dms(double dd, uint32_t& degrees, uint32_t& minutes, double& seconds)
{
    degrees = static_cast<uint32_t>(dd);

    double mins = (dd - static_cast<double>(degrees)) * 60.;
    minutes     = static_cast<uint32_t>(mins);

    seconds = (mins - static_cast<double>(minutes)) * 60.;
}

/** @brief Convert a coordinate from decimal degrees (DD) to degrees minutes seconds (DMS) representation */
void i_gnss::to_dms(double dd, uint32_t& degrees, uint32_t& minutes, uint32_t& seconds)
{
    degrees = static_cast<uint32_t>(dd);

    double mins = (dd - static_cast<double>(degrees)) * 60.;
    minutes     = static_cast<uint32_t>(mins);

    double secs = (mins - static_cast<double>(minutes)) * 60.;
    seconds     = static_cast<uint32_t>(secs);
}

/** @brief Convert a coordinate from degrees minutes seconds (DMS) to decimal degrees (DD) representation */
void i_gnss::to_dd(uint32_t degrees, uint32_t minutes, double seconds, double& dd)
{
    dd = static_cast<double>(degrees) + static_cast<double>(minutes) / 60. + seconds / 3600.;
}

/** @brief Convert a coordinate from degrees minutes seconds (DMS) to decimal degrees (DD) representation */
void i_gnss::to_dd(uint32_t degrees, uint32_t minutes, uint32_t seconds, double& dd)
{
    dd = static_cast<double>(degrees) + static_cast<double>(minutes) / 60. + static_cast<double>(seconds) / 3600.;
}

} // namespace ov
