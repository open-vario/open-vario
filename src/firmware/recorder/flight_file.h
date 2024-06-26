/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_FLIGHT_FILE_H
#define OV_FLIGHT_FILE_H

#include "date_time.h"
#include "file.h"
#include "i_barometric_altimeter.h"
#include "i_gnss.h"

namespace ov
{

/** @brief Flight file */
class flight_file
{
  public:
    /** @brief Header */
    struct header
    {
        /** @brief Timestamp */
        date_time timestamp;
        /** @brief Glider */
        char glider[32u];
        /** @brief Entry period in milliseconds */
        uint16_t period;

        /** @brief Magic number value */
        static constexpr uint32_t MAGIC_NUMBER = 0xBEEFF00Du;
    };

    /** @brief Flight file entry */
    struct entry
    {
        // GNSS data

        /** @brief Latitude (1 = 1°) */
        double latitude;
        /** @brief Longitude (1 = 1°) */
        double longitude;
        /** @brief Speed (1 = 0.1 m/s) */
        uint32_t speed;
        /** @brief Altitude (1 = 0.1 m) */
        uint32_t gnss_altitude;

        // Altimeter data

        /** @brief Pressure (1 = 0.01mbar) */
        int32_t pressure;
        /** @brief Altitude (1 = 0.1m) */
        int32_t altitude;
        /** @brief Temperature (1 = 0.1°C) */
        int16_t temperature;

        // Accelerometer data

        /** @brief Total acceleration (1000 = 1g) */
        int16_t total_accel;

        // Computed data

        /** @brief Sink rate (1 = 0.1m/s) */
        int16_t sink_rate;
        /** @brief Glide ratio (1 = 0.1) */
        uint16_t glide_ratio;

        // Data validity

        /** @brief Indicate if the GNSS data is valid */
        bool gnss_is_valid;
        /** @brief Indicate if the altimeter data is valid */
        bool alti_is_valid;
        /** @brief Indicate if the accelerometer data is valid */
        bool accel_is_valid;
    };

    /** @brief Constructor to open the file for read operations */
    flight_file(const char* path);

    /** @brief Constructor to open the file for write operations */
    flight_file(const char* path, header& header);

    /** @brief Close the file */
    bool close();

    /** @brief Indicate if the file is valid */
    bool is_open() const { return m_file.is_open(); }

    /** @brief Get the file header */
    const header& get_header() const { return m_header; }

    /** @brief Write a flight entry to the file */
    bool write(const entry& e);

    /** @brief Read a flight entry from the file */
    bool read(entry& e);

  protected:
    /** @brief Header */
    header m_header;
    /** @brief File handle */
    file m_file;
};

} // namespace ov

#endif // OV_FLIGHT_FILE_H
