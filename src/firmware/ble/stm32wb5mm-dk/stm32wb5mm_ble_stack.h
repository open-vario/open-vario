
#ifndef OV_STM32WB5MM_BLE_STACK_H
#define OV_STM32WB5MM_BLE_STACK_H

#include "i_ble_stack.h"
#include "svc_ctl.h"
#include "thread.h"

namespace ov
{

/** @brief BLE stack implementation for STM32WB5MM target */
class stm32wb5mm_ble_stack : public i_ble_stack
{
  public:
    /** @brief Constructor */
    stm32wb5mm_ble_stack();

    /** @brief Start the stack */
    bool start(i_ble_service* services[], size_t services_count);

    /** @brief Indicate if the stack is started */
    bool is_started() override { return m_is_started; }

    /** @brief Indicate if the stack is ready */
    bool is_ready() override { return m_is_ready; }

    /** @brief Indicate if a device is connected */
    bool is_device_connected() override;

  private:
    /** @brief BLE thread */
    thread<2048u> m_thread;
    /** @brief List of BLE services */
    i_ble_service** m_services;
    /** @brief Number of BLE services */
    size_t m_services_count;
    /** @brief Indicate that the stack is started */
    bool m_is_started;
    /** @brief Indicate that the stack is ready */
    bool m_is_ready;

    /** @brief BLE thread */
    void thread_func(void*);
    /** @brief Create a BLE service */
    void create_service(i_ble_service* service);
    /** @brief Find a BLE characteristic from its handle */
    i_ble_characteristic* find_characteristic(uint16_t handle);
    /** @brief Handler called when a characteristic value must be updated in the BLE stack */
    bool on_characteristic_updated_handler(i_ble_characteristic& characteristic, const void* value, size_t new_size);

    /** @brief Service event handler */
    static SVCCTL_EvtAckStatus_t service_event_handler(void* p_evt);
};

} // namespace ov

#endif // OV_STM32WB5MM_BLE_STACK_H
