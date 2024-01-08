
#include "stm32hal_iopin.h"

namespace ov
{

/** @brief Constructor */
stm32hal_iopin::stm32hal_iopin(GPIO_TypeDef* port, uint32_t pin) : m_port(port), m_pin(pin) { }

/** @brief Get the pin level */
io::level stm32hal_iopin::get_level()
{
    return static_cast<io::level>(HAL_GPIO_ReadPin(m_port, m_pin));
}

/** @brief Set the pin to a specified level */
void stm32hal_iopin::set_level(io::level level)
{
    HAL_GPIO_WritePin(m_port, m_pin, static_cast<GPIO_PinState>(level));
}

} // namespace ov
