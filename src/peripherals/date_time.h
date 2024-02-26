/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_DATE_TIME_H
#define OV_DATE_TIME_H

#include <cstdint>

namespace ov
{

/** @brief Date and time */
struct date_time
{
    /** @brief Year (since 2000) */
    uint8_t year;
    /** @brief Month (1 - 12) */
    uint8_t month;
    /** @brief Day (1 - 31) */
    uint8_t day;
    /** @brief Hour (0 - 24) */
    uint8_t hour;
    /** @brief Minute (0 - 59) */
    uint8_t minute;
    /** @brief Second (0 - 59) */
    uint8_t second;
    /** @brief Milliseconds (0 - 999) */
    uint16_t millis;
};

} // namespace ov

#endif // OV_DATE_TIME_H
