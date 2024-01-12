
#include "flight_recorder.h"
#include "fs.h"
#include "os.h"
#include "ov_data.h"

#include <cstdio>

namespace ov
{

/** @brief Constructor */
flight_recorder::flight_recorder() : m_status(status::stopped), m_recording_start(0u), m_thread() { }

/** @brief Initialize the recorder */
bool flight_recorder::init()
{
    bool ret = true;

    // Create the directory to store the recorded data
    dir storage_dir = fs::open_dir(RECORDED_DATA_DIR);
    if (!storage_dir.is_open())
    {
        ret = fs::mkdir(RECORDED_DATA_DIR);
    }
    if (ret)
    {
        // Start recording thread
        auto thread_func = ov::thread_func::create<flight_recorder, &flight_recorder::thread_func>(*this);
        ret              = m_thread.start(thread_func, "Recorder", 3u, nullptr);
    }

    return ret;
}

/** @brief Start recording */
bool flight_recorder::start()
{
    bool ret = false;

    // Check if already started
    if (m_status == status::stopped)
    {
        // Indicate start of recording
        m_status = status::starting;

        ret = true;
    }

    return ret;
}

/** @brief Stop recording */
bool flight_recorder::stop()
{
    bool ret = false;

    // Check if started
    if (m_status == status::started)
    {
        // Indicate end of recording
        m_status = status::stopping;

        ret = true;
    }

    return ret;
}

/** @brief Get the recording duration in seconds */
uint32_t flight_recorder::get_recording_duration()
{
    // Compute duration
    uint32_t now      = os::now();
    uint32_t duration = (now - m_recording_start) / configTICK_RATE_HZ;
    return duration;
}

/** @brief Recorder thread */
void flight_recorder::thread_func(void*)
{
    char    filepath[64u];
    ov_data data;

    // Thread loop
    while (true)
    {
        // Wait start
        while (m_status == status::stopped)
        {
            ov::this_thread::sleep_for(250u);
        }
        ov::this_thread::sleep_for(1000u);

        // Create flight file path
        data = ov::data::get();
        if (data.gnss.is_valid)
        {
            snprintf(filepath,
                     sizeof(filepath),
                     "%s/%04d-%02d-%02dT%02d-%02d-%02d.rec",
                     RECORDED_DATA_DIR,
                     static_cast<int>(data.gnss.date.year) + 2000,
                     static_cast<int>(data.gnss.date.month),
                     static_cast<int>(data.gnss.date.day),
                     static_cast<int>(data.gnss.date.hour),
                     static_cast<int>(data.gnss.date.minute),
                     static_cast<int>(data.gnss.date.second));
        }
        else
        {
            uint32_t now = os::now();
            snprintf(filepath, sizeof(filepath), "%s/%06d.rec", RECORDED_DATA_DIR, static_cast<int>(now));
        }

        // Create flight file
        file flight_file = fs::open(filepath, ov::fs::o_creat | ov::fs::o_trunc | ov::fs::o_wronly);
        if (flight_file.is_open())
        {
            // Recorder is now started
            m_recording_start = os::now();
            m_status          = status::started;

            // Wait stop
            while (m_status != status::stopping)
            {
                // Record current flight data
                size_t write_count = 0;
                flight_file.write(&data.gnss.is_valid, sizeof(data.gnss.is_valid), write_count);
                flight_file.write(&data.gnss.altitude, sizeof(data.gnss.altitude), write_count);
                flight_file.write(&data.gnss.latitude, sizeof(data.gnss.latitude), write_count);
                flight_file.write(&data.gnss.longitude, sizeof(data.gnss.longitude), write_count);
                flight_file.write(&data.gnss.speed, sizeof(data.gnss.speed), write_count);
                flight_file.write(&data.gnss.track_angle, sizeof(data.gnss.track_angle), write_count);
                flight_file.write(&data.altimeter.is_valid, sizeof(data.altimeter.is_valid), write_count);
                flight_file.write(&data.altimeter.pressure, sizeof(data.altimeter.pressure), write_count);
                flight_file.write(&data.altimeter.temperature, sizeof(data.altimeter.temperature), write_count);
                flight_file.write(&data.altimeter.altitude, sizeof(data.altimeter.altitude), write_count);

                // Recording period
                ov::this_thread::sleep_for(1000u);
            }

            // Close flight file
            flight_file.close();
            ov::this_thread::sleep_for(1000u);
        }

        // Recorder is now stopped
        m_status = status::stopped;
    }
}

} // namespace ov
