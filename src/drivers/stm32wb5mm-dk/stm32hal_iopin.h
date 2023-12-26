
#ifndef OV_STM32HAL_IOPIN_H
#define OV_STM32HAL_IOPIN_H

#include "i_input_pin.h"
#include "i_output_pin.h"

#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_gpio.h"

namespace ov
{

/** @brief Input/output pins driver implementation using STM32HAL */
class stm32hal_iopin : public i_input_pin, public i_output_pin
{
  public:
    /** @brief Constructor */
    stm32hal_iopin(GPIO_TypeDef* port, uint32_t pin);

    /** @brief Indicate if the pin level is low */
    bool is_low() override { return (get_level() == LOW); }

    /** @brief Indicate if the pin level is high */
    bool is_high() override { return (get_level() == HIGH); }

    /** @brief Get the pin level */
    Level get_level() override;

    /** @brief Set the pin to low level */
    void set_low() override { set_level(LOW); }

    /** @brief Set the pin to high level */
    void set_high() override { set_level(HIGH); }

    /** @brief Set the pin to a specified level */
    void set_level(Level level) override;

  private:
    /** @brief Port */
    GPIO_TypeDef* m_port;
    /** @brief Pin */
    uint32_t m_pin;
};

} // namespace ov

#endif // OV_STM32HAL_IOPIN_H
