
#include "os.h"

#include "FreeRTOS.h"
#include "task.h"

namespace ov
{
namespace os
{

/** @brief Starts the operating system */
void start()
{
    // Start FreeRTOS
    vTaskStartScheduler();
}

} // namespace os
} // namespace ov
