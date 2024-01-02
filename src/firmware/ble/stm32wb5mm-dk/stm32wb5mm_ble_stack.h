
#ifndef OV_STM32WB5MM_BLE_STACK_H
#define OV_STM32WB5MM_BLE_STACK_H

#include "i_ble_stack.h"
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
    bool start();

  private:
    /** @brief BLE thread */
    thread<2048u> m_thread;

    /** @brief BLE thread */
    void thread_func(void*);
};

} // namespace ov

#endif // OV_STM32WB5MM_BLE_STACK_H
