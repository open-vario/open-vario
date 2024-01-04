
#ifndef OV_BLE_MANAGER_H
#define OV_BLE_MANAGER_H

#include "thread.h"

#include "ble_config_service.h"

namespace ov
{

// Forward declaration
class i_ble_stack;

/** @brief BLE manager */
class ble_manager
{
  public:
    /** @brief Constructor */
    ble_manager(i_ble_stack& ble_stack);

    /** @brief Start the BLE manager */
    bool start();

  private:
    /** @brief BLE stack */
    i_ble_stack& m_ble_stack;
    /** @brief BLE update thread */
    thread<2048u> m_thread;
    /** @brief BLE services */
    i_ble_service* m_services[1u];

    /** @brief Configuration service */
    ble_config_service m_config_service;

    /** @brief BLE update thread */
    void thread_func(void*);
};

} // namespace ov

#endif // OV_BLE_MANAGER_H
