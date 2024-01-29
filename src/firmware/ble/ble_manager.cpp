
#include "ble_manager.h"

namespace ov
{

/** @brief Constructor */
ble_manager::ble_manager(i_ble_stack& ble_stack) : m_ble_stack(ble_stack), m_thread(), m_services(), m_config_service()
{
    // Fill services array
    m_services[0u] = &m_config_service.get_service();
}

/** @brief Start the BLE manager */
bool ble_manager::start()
{
    // Start BLE stack
    bool ret = m_ble_stack.start(m_services, sizeof(m_services) / sizeof(i_ble_service*));
    if (ret)
    {
        // Start thread
        auto thread_func = ov::thread_func::create<ble_manager, &ble_manager::thread_func>(*this);
        ret              = m_thread.start(thread_func, "BLE MGR", 5u, nullptr);
    }

    return ret;
}

/** @brief BLE update thread */
void ble_manager::thread_func(void*)
{
    // Wait for the stack to be ready
    while (!m_ble_stack.is_ready())
    {
        ov::this_thread::sleep_for(50u);
    }

    // Init values for the characteristics
    m_config_service.set_init_values();

    // Thread loop
    while (true)
    {
        // Update characteristics

        // Wait next period
        ov::this_thread::sleep_for(500u);
    }
}

} // namespace ov
