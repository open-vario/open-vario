/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_FLIGHT_RECORDER_H
#define OV_FLIGHT_RECORDER_H

#include "i_flight_recorder.h"
#include "thread.h"

namespace ov
{

/** @brief Flight recorder */
class flight_recorder : public i_flight_recorder
{
  public:
    /** @brief Constructor */
    flight_recorder();

    /** @brief Initialize the recorder */
    bool init();

    /** @brief Start recording */
    bool start() override;

    /** @brief Stop recording */
    bool stop() override;

    /** @brief Get the status of the recorder */
    status get_status() override { return m_status; }

    /** @brief Get the recording duration in seconds */
    uint32_t get_recording_duration() override;

  protected:
    /** @brief Status of the recorder */
    status m_status;
    /** @brief Timestamp of the start of recording in milliseconds */
    uint32_t m_recording_start;
    /** @brief Recorder thread */
    thread<2048u> m_thread;

    /** @brief Recorder thread */
    void thread_func(void*);
};

} // namespace ov

#endif // OV_FLIGHT_RECORDER_H
