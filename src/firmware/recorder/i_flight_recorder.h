/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_FLIGHT_RECORDER_H
#define OV_I_FLIGHT_RECORDER_H

#include <cstdint>

namespace ov
{

/** @brief Interface for the flight recorder implementation */
class i_flight_recorder
{
  public:
    /** @brief Status of the recorder */
    enum class status
    {
        /** @brief Error */
        stopped_error,
        /** @brief Stopped */
        stopped,
        /** @brief Starting */
        starting,
        /** @brief Started */
        started,
        /** @brief Error */
        started_error,
        /** @brief Stopping */
        stopping
    };

    /** @brief Destructor */
    virtual ~i_flight_recorder() { }

    /** @brief Start recording */
    virtual bool start() = 0;

    /** @brief Stop recording */
    virtual bool stop() = 0;

    /** @brief Get the status of the recorder */
    virtual status get_status() = 0;

    /** @brief Get the recording duration in seconds */
    virtual uint32_t get_recording_duration() = 0;

    /** @brief Directory to store the recorded data */
    static constexpr const char* RECORDED_DATA_DIR = "/flights";
    /** @brief Extension for flight files */
    static constexpr const char* RECORDED_DATA_EXT = ".rec";
};

} // namespace ov

#endif // OV_I_FLIGHT_RECORDER_H
