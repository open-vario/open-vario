
#ifndef OV_BLE_MANAGER_H
#define OV_BLE_MANAGER_H

#include "i_ble_manager.h"
#include "thread.h"

#include "ble_config_service.h"

namespace ov
{

/** @brief BLE manager */
class ble_manager : public i_ble_manager
{
  public:
    /** @brief Constructor */
    ble_manager(i_ble_stack& ble_stack);

    /** @brief Start the BLE manager */
    bool start() override;

    /** @brief Indicate if the BLE manager is started */
    bool is_started() override { return m_ble_stack.is_started(); }

    /** @brief Get the BLE stack */
    i_ble_stack& ble_stack() override { return m_ble_stack; }

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
