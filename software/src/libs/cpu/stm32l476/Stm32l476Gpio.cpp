/*
Copyright(c) 2017 Cedric Jimenez

This file is part of Open-Vario.

Open-Vario is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Open-Vario is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Open-Vario.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Stm32l476Gpio.h"

#include "stm32l476xx.h"

namespace open_vario
{

/** \brief Register definitions for the GPIO ports */
static GPIO_TypeDef* const gpio_regs[] = { GPIOA, GPIOB, GPIOC, GPIOD };


/** \brief Constructor */
Stm32l476Gpio::Stm32l476Gpio(const Port port, const uint8_t pin, const Mode mode, const uint8_t alternate_function, const uint8_t configuration, const Speed speed)
: m_port(port)
, m_pin(pin)
, m_mode(mode)
, m_alternate_function(alternate_function)
, m_configuration(configuration)
, m_speed(speed)
{}

/** \brief Configure the pin */
bool Stm32l476Gpio::configure()
{
    bool ret = false;

    /* Check port and pin validity */
    if ((m_port < PORT_MAX) && (m_pin < MAX_PIN_PER_PORT))
    {
        /* Get port register */
        const uint8_t pin_bit_mask = (1u << m_pin);
        const uint8_t pin_bit_pos = 2u * m_pin;
        GPIO_TypeDef* const gpio_port = gpio_regs[m_port];

        /* Enable GPIO clock */
        RCC->AHB2ENR |= (1u << m_port);

        /* Configure mode */
        switch (m_mode)
        {
            case MODE_INPUT:
            /* Intended fallthrough */            
            case MODE_OUTPUT:
            /* Intended fallthrough */
            case MODE_AF:
            /* Intended fallthrough */
            case MODE_ANALOG:
            {
                uint32_t reg = gpio_port->MODER;
                reg &= ~(3u << pin_bit_pos);
                gpio_port->MODER = reg | (static_cast<uint8_t>(m_mode) << pin_bit_pos);
                
                ret = true;
                break;    
            }

            default:
            {
                /* Invalid mode */
                ret = false;
            }
        }

        /* Configure alternate function */
        if (m_alternate_function < MAX_AF_PER_PIN)
        {
            if (m_pin < 8u)
            {
                const uint8_t pin_bit_pos = 4u * m_pin;
                gpio_port->AFR[0] &= ~(0x0F << pin_bit_pos);
                gpio_port->AFR[0] |= (m_alternate_function << pin_bit_pos);
            }
            else
            {
                const uint8_t pin_bit_pos = 4u * (m_pin - 8u);
                gpio_port->AFR[1] &= ~(0x0F << pin_bit_pos);
                gpio_port->AFR[1] |= (m_alternate_function << pin_bit_pos);
            }
        }
        else
        {
            ret = false;
        }

        /* Configure speed */
        gpio_port->OSPEEDR &= ~(3u << pin_bit_pos);
        gpio_port->OSPEEDR |= ((static_cast<uint8_t>(m_speed) & 3u) << pin_bit_pos);

        /* Pin configuration */
        if ((m_configuration & CONFIG_OPEN_DRAIN) == 0u)
        {
            gpio_port->OTYPER &= ~pin_bit_mask;
        }
        else
        {
            gpio_port->OTYPER |= pin_bit_mask;            
        }
        gpio_port->PUPDR &= ~(3u << pin_bit_pos);
        if ((m_configuration & CONFIG_PULL_UP) != 0u)
        {
            gpio_port->PUPDR |= (1u << pin_bit_pos);
        }
        else
        {
            if ((m_configuration & CONFIG_PULL_DOWN) != 0u)
            {
                gpio_port->PUPDR |= (2u << pin_bit_pos);
            }
        }
    }

    return ret;
}

/** \brief Indicate if the pin level is low */
bool Stm32l476Gpio::isLow()
{
    const uint8_t pin_bit_mask = (1u << m_pin);
    GPIO_TypeDef* const gpio_port = gpio_regs[m_port];
    return ((gpio_port->IDR & pin_bit_mask) == 0u);
}

/** \brief Indicate if the pin level is high */
bool Stm32l476Gpio::isHigh()
{
    return !isLow();
}

/** \brief Get the pin level */
IIoPin::Level Stm32l476Gpio::getLevel()
{
    return isHigh();
}

/** \brief Set the pin to low level */
void Stm32l476Gpio::setLow()
{
    GPIO_TypeDef* const gpio_port = gpio_regs[m_port];
    gpio_port->BSRR = (1u << (m_pin + 16u));
}

/** \brief Set the pin to high level */
void Stm32l476Gpio::setHigh()
{
    GPIO_TypeDef* const gpio_port = gpio_regs[m_port];
    gpio_port->BSRR = (1u << m_pin);
}

/** \brief Set the pin to a specified level */
void Stm32l476Gpio::setLevel(const Level level)
{
    if (level == HIGH)
    {
        setHigh();
    }
    else
    {
        setLow();
    }
}


}
