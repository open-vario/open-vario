
#include "ov_app.h"
#include "fs.h"
#include "mean_filter.h"
#include "os.h"
#include "ov_config.h"
#include "ov_data.h"

#include <stdio.h>

namespace ov
{

/** @brief Constructor */
ov_app::ov_app()
    : m_board(),
      m_console(m_board, m_board.get_debug_port()),
      m_fs_console(m_console),
      m_config_console(m_console),
      m_sensors_console(m_console, m_board.get_altimeter()),
      m_recorder_console(m_console, m_recorder),
      m_hmi(m_board.get_display(),
            m_console,
            m_board.get_previous_button(),
            m_board.get_next_button(),
            m_board.get_select_button(),
            m_ble,
            m_xctrack,
            m_recorder),
      m_ble(m_board.get_ble_stack()),
      m_recorder(),
      m_xctrack(m_board.get_usb_cdc()),
      m_maintenance(m_board.get_usb_cdc()),
      m_thread()
{
}

/** @brief Initialize application */
bool ov_app::init()
{
    // Configure clocks
    bool ret = m_board.clock_init();
    return ret;
}

/** @brief Start the application (shall not return in case of sucess) */
void ov_app::start()
{
    // Create main thread
    auto thread_func = ov::thread_func::create<ov_app, &ov_app::thread_func>(*this);
    if (m_thread.start(thread_func, "Main", 7u, nullptr))
    {
        // Start operating system
        ov::os::start();
    }

    // Shall never happen
    return;
}

/** @brief Main thread */
void ov_app::thread_func(void*)
{
    // Initialize application
    startup();

    // Sensors data
    auto& gnss          = m_board.get_gnss();
    auto& altimeter     = m_board.get_altimeter();
    auto& accelerometer = m_board.get_accelerometer();

    i_gnss::data                 gnss_data;
    i_barometric_altimeter::data baro_data;
    i_accelerometer_sensor::data accel_data;

    // Filters for sink rate and glide ratio computation
    int32_t                            sink_rate_previous_altitude = 0;
    mean_filter<int16_t, int32_t, 40u> sink_rate_filter;

    uint32_t                       glide_ratio_period_ms = 0u;
    circular_buffer<int32_t, 15u>  glide_ratio_altitudes;
    circular_buffer<uint16_t, 15u> glide_ratio_distances;

    // Main loop
    constexpr uint32_t sensor_period_ms = 250u;
    while (true)
    {
        // Get gnss data
        if (gnss.update_data())
        {
            gnss_data = gnss.get_data();
            ov::data::set_gnss(gnss_data);
        }
        else
        {
            gnss_data.is_valid = false;
            ov::data::invalidate_gnss();
        }

        // Get barometric altimeter data
        baro_data = altimeter.get_data();
        ov::data::set_altimeter(baro_data);

        // Get accelerometer data
        accel_data = accelerometer.get_data();
        ov::data::set_accelerometer(accel_data);

        // Filters depths
        size_t sink_rate_depth   = ov::config::get().sr_integ_time / sensor_period_ms;
        size_t glide_ratio_depth = ov::config::get().gr_integ_time / 1000u;
        if (sink_rate_filter.get_depth() != sink_rate_depth)
        {
            sink_rate_filter.set_depth(sink_rate_depth);
        }
        if (glide_ratio_altitudes.get_depth() != glide_ratio_depth)
        {
            glide_ratio_altitudes.set_depth(glide_ratio_depth);
            glide_ratio_distances.set_depth(glide_ratio_depth);
        }

        // Compute sink rate
        int32_t delta_alti          = baro_data.altitude - sink_rate_previous_altitude;
        sink_rate_previous_altitude = baro_data.altitude;

        int16_t sink_rate      = static_cast<int16_t>((delta_alti * 1000) / static_cast<int32_t>(sensor_period_ms));
        auto    mean_sink_rate = sink_rate_filter.add_value(sink_rate);
        ov::data::set_sink_rate(mean_sink_rate);

        // Compute glide ratio every second
        glide_ratio_period_ms += sensor_period_ms;
        if (sensor_period_ms == 1000u)
        {
            int32_t previous_altitude = glide_ratio_altitudes.get_oldest_value();
            delta_alti                = baro_data.altitude - previous_altitude;
            glide_ratio_altitudes.add_value(baro_data.altitude);
            if (gnss_data.is_valid)
            {
                glide_ratio_distances.add_value(static_cast<uint16_t>(gnss_data.speed));
            }
            else
            {
                glide_ratio_distances.add_value(glide_ratio_distances.get_oldest_value());
            }

            // Glide ratio validity
            uint16_t glide_ratio = ov_data::INVALID_GLIDE_RATIO_VALUE;
            if (gnss_data.is_valid && (delta_alti > 0))
            {
                uint16_t cumulated_distance = glide_ratio_distances.sum();
                glide_ratio                 = cumulated_distance / static_cast<uint16_t>(delta_alti);
            }
            ov::data::set_glide_ratio(glide_ratio);

            // Reset computation period
            glide_ratio_period_ms = 0u;
        }

        ov::this_thread::sleep_for(sensor_period_ms);
    }
}

/** @brief Startup process */
void ov_app::startup()
{
    // Init board
    m_board.init();

    // Init HMI
    m_hmi.start();

    // Init filesystem
    bool fs_reinitialized = false;
    ov::fs::init(fs_reinitialized, m_board.get_storage_memory());

    // Load configuration
    if (!ov::config::load())
    {
        // Save default values
        ov::config::save();
    }

    // Register custom console commands
    m_fs_console.register_handlers();
    m_config_console.register_handlers();
    m_sensors_console.register_handlers();
    m_recorder_console.register_handlers();

    // Start console
    m_console.start();

    // Initialize recorder
    m_recorder.init();

    // Load altimeter with calibration data
    const auto& config = ov::config::get();
    m_board.get_altimeter().set_references(config.alti_ref_temp, config.alti_ref_pressure, config.alti_ref_alti);

    // Start XCTrack link
    m_xctrack.init();

    // Start maintenance link
    m_maintenance.init();
}

} // namespace ov
