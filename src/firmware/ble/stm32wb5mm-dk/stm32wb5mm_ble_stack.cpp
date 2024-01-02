
#include "stm32wb5mm_ble_stack.h"
#include "app_ble.h"
#include "stm32_seq.h"

namespace ov
{

/** @brief Constructor */
stm32wb5mm_ble_stack::stm32wb5mm_ble_stack() : m_thread() { }

/** @brief Start the stack */
bool stm32wb5mm_ble_stack::start()
{
    // Start thread
    auto thread_func = ov::thread_func::create<stm32wb5mm_ble_stack, &stm32wb5mm_ble_stack::thread_func>(*this);
    bool ret         = m_thread.start(thread_func, "BLE", 3u, nullptr);

    return ret;
}

/** @brief BLE thread */
void stm32wb5mm_ble_stack::thread_func(void*)
{
    // Start BLE stack
    APPE_Tl_Init();

    // Thread loop
    while (true)
    {
        UTIL_SEQ_Run(0xFFFFFFFFu);
    }
}

} // namespace ov

/** @brief Idle task for BLE */
extern "C" void UTIL_SEQ_Idle()
{
    ov::this_thread::sleep_for(100u);
}
