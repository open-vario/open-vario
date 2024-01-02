
#include "ov_app.h"
#include "fs.h"
#include "os.h"
#include "ov_config.h"

namespace ov
{

/** @brief Constructor */
ov_app::ov_app()
    : m_board(),
      m_console(m_board.get_debug_port()),
      m_fs_console(m_console),
      m_config_console(m_console),
      m_hmi(m_board.get_display(), m_console, m_board.get_previous_button(), m_board.get_next_button(), m_board.get_select_button()),
      m_thread()
{
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
    init();

    // Main loop
    while (true)
    {
        ov::this_thread::sleep_for(1000u);
    }
}

/** @brief Init process */
void ov_app::init()
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

    // Start console
    m_console.start();

    // Start BLE stack
    m_board.get_ble_stack().start();
}

} // namespace ov
