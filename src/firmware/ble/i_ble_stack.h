
#ifndef OV_I_BLE_STACK_H
#define OV_I_BLE_STACK_H

namespace ov
{

/** @brief Interface for the BLE stack implementation */
class i_ble_stack
{
  public:
    /** @brief Destructor */
    virtual ~i_ble_stack() { }

    /** @brief Start the stack */
    virtual bool start() = 0;

    /** @brief Indicate if a device is connected */
    virtual bool is_device_connected() = 0;
};

} // namespace ov

#endif // OV_I_BLE_STACK_H
