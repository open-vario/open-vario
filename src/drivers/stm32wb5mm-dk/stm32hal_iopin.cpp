
#include "stm32hal_iopin.h"

namespace ov
{

/** @brief Constructor */
stm32hal_iopin::stm32hal_iopin(GPIO_TypeDef* port, uint32_t pin) : m_port(port), m_pin(pin) { }

/** @brief Get the pin level */
i_io_pin::Level stm32hal_iopin::get_level()
{
    return static_cast<Level>(HAL_GPIO_ReadPin(m_port, m_pin));
}

/** @brief Set the pin to a specified level */
void stm32hal_iopin::set_level(i_io_pin::Level level)
{
    HAL_GPIO_WritePin(m_port, m_pin, static_cast<GPIO_PinState>(level));
}

} // namespace ov
