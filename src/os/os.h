
#ifndef OV_OS_H
#define OV_OS_H

#include <cstdint>

namespace ov
{
namespace os
{

/** @brief Starts the operating system */
void start();

/** @brief Yield from interrupt */
void yield_from_isr(bool higher_priority_task_woken);

/** @brief Get the infinite timeout value */
uint32_t infinite_timeout_value();

} // namespace os
} // namespace ov

#endif // OV_OS_H
