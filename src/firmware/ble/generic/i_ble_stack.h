
#ifndef OV_I_BLE_STACK_H
#define OV_I_BLE_STACK_H

#include "i_ble_service.h"

namespace ov
{

/** @brief Interface for the BLE stack implementation */
class i_ble_stack
{
  public:
    /** @brief Destructor */
    virtual ~i_ble_stack() { }

    /** @brief Start the stack */
    virtual bool start(i_ble_service* services[], size_t services_count) = 0;

    /** @brief Indicate if the stack is ready */
    virtual bool is_ready() = 0;

    /** @brief Indicate if a device is connected */
    virtual bool is_device_connected() = 0;
};

} // namespace ov

#endif // OV_I_BLE_STACK_H
