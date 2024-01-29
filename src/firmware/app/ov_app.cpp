
#include "ov_app.h"
#include "fs.h"
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

    // Main loop
    auto& gnss          = m_board.get_gnss();
    auto& altimeter     = m_board.get_altimeter();
    auto& accelerometer = m_board.get_accelerometer();
    while (true)
    {
        // Get gnss data
        if (gnss.update_data())
        {
            ov::data::set_gnss(gnss.get_data());
        }
        else
        {
            ov::data::invalidate_gnss();
        }

        // Get barometric altimeter data
        ov::data::set_altimeter(altimeter.get_data());

        // Get accelerometer data
        ov::data::set_accelerometer(accelerometer.get_data());

        ov::this_thread::sleep_for(100u);
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
