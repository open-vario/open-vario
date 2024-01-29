
#ifndef OV_I_BLE_MANAGER_H
#define OV_I_BLE_MANAGER_H

#include "i_ble_stack.h"

namespace ov
{

/** @brief Interface for the BLE manager */
class i_ble_manager
{
  public:
    /** @brief Destructor */
    virtual ~i_ble_manager() { }

    /** @brief Start the BLE manager */
    virtual bool start() = 0;

    /** @brief Indicate if the BLE manager is started */
    virtual bool is_started() = 0;

    /** @brief Get the BLE stack */
    virtual i_ble_stack& ble_stack() = 0;
};

} // namespace ov

#endif // OV_I_BLE_MANAGER_H
