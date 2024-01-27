
#include "flight_recorder.h"
#include "flight_file.h"
#include "fs.h"
#include "os.h"
#include "ov_config.h"
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
        ret              = m_thread.start(thread_func, "Recorder", 4u, nullptr);
    }

    return ret;
}

/** @brief Start recording */
bool flight_recorder::start()
{
    bool ret = false;

    // Check if already started
    if (m_status <= status::stopped)
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
    if ((m_status == status::started) || (m_status == status::started_error))
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
    char             filepath[64u];
    ov_data          data;
    const ov_config& config = ov::config::get();

    // Thread loop
    while (true)
    {
        // Wait start
        while (m_status <= status::stopped)
        {
            ov::this_thread::sleep_for(250u);
        }
        ov::this_thread::sleep_for(1000u);

        // Save recording period so that it cannot change during the flight
        uint32_t recording_period = config.recording_period;

        // Create flight file path
        data = ov::data::get();
        if (data.gnss.is_valid)
        {
            snprintf(filepath,
                     sizeof(filepath),
                     "%s/%04d-%02d-%02dT%02d-%02d-%02d%s",
                     RECORDED_DATA_DIR,
                     static_cast<int>(data.gnss.date.year) + 2000,
                     static_cast<int>(data.gnss.date.month),
                     static_cast<int>(data.gnss.date.day),
                     static_cast<int>(data.gnss.date.hour),
                     static_cast<int>(data.gnss.date.minute),
                     static_cast<int>(data.gnss.date.second),
                     RECORDED_DATA_EXT);
        }
        else
        {
            uint32_t now = os::now();
            snprintf(filepath, sizeof(filepath), "%s/%06d.rec", RECORDED_DATA_DIR, static_cast<int>(now));
        }

        // Create flight file
        flight_file::header header = {};
        const char*         glider_name;
        header.timestamp = data.gnss.date;
        switch (config.glider)
        {
            case 1:
                [[fallthrough]];
            default:
                glider_name = config.glider1_name;
                break;
            case 2:
                glider_name = config.glider2_name;
                break;
            case 3:
                glider_name = config.glider3_name;
                break;
            case 4:
                glider_name = config.glider4_name;
                break;
        }
        strcpy(header.glider, glider_name);
        header.period = recording_period;

        flight_file flight(filepath, header);
        if (flight.is_open())
        {
            // Recorder is now started
            m_recording_start = os::now();
            m_status          = status::started;

            // Wait stop
            while (m_status != status::stopping)
            {
                // Get flight data
                data = ov::data::get();

                // Write a new entry
                flight_file::entry entry;
                entry.gnss_is_valid  = data.gnss.is_valid;
                entry.latitude       = data.gnss.latitude;
                entry.longitude      = data.gnss.longitude;
                entry.speed          = data.gnss.speed;
                entry.altitude       = data.gnss.altitude;
                entry.altimeter      = data.altimeter;
                entry.accel_is_valid = data.accelerometer.is_valid;
                entry.total_accel    = data.accelerometer.total_accel;
                if (!flight.write(entry))
                {
                    // Error
                    m_status = status::started_error;
                }

                // Recording period
                ov::this_thread::sleep_for(recording_period);
            }

            // Close flight file
            if (flight.close())
            {
                // Recorder is now stopped
                ov::this_thread::sleep_for(1000u);
                m_status = status::stopped;
            }
            else
            {
                // Error
                m_status = status::stopped_error;
            }
        }
        else
        {
            // Error
            m_status = status::stopped_error;
        }
    }
}

} // namespace ov
