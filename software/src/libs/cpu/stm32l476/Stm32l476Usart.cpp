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

#include "Stm32l476Usart.h"
#include "Stm32l476Cpu.h"

#include "stm32l476xx.h"

#include <cstddef>

namespace open_vario
{

/** \brief Register definitions for the USARTs */
static USART_TypeDef* const usart_regs[] = { USART1, USART2, USART3 };

/** \brief USART instances */
static Stm32l476Usart* usarts[Stm32l476Usart::USART_MAX] = {NULL, NULL, NULL};


/** \brief Constructor */
Stm32l476Usart::Stm32l476Usart(const Stm32l476Cpu& cpu, const Usart usart, const uint32_t baudrate, const Parity parity, const StopBits stop_bits, const FlowControl flow_control)
: m_cpu(cpu)
, m_usart(usart)
, m_baudrate(baudrate)
, m_parity(parity)
, m_stop_bits(stop_bits)
, m_flow_control(flow_control)
{
    // Register instance
    usarts[usart] = this;
}

/** \brief Configure the UART */
bool Stm32l476Usart::configure()
{
    bool ret = true;

    // Enable clock and configure to use PCLK as input clock
    switch (m_usart)
    {
        case USART_1:
        {
            RCC->APB2ENR |= (1u << 14u);
            RCC->CCIPR &= ~(3u  << 0u);
            break;
        }

        case USART_2:
        {
            RCC->APB1ENR1 |= (1u << 17u);
            RCC->CCIPR &= ~(3u  << 2u);
            break;
        }

        case USART_3:
        {
            RCC->APB1ENR1 |= (1u << 18u);
            RCC->CCIPR &= ~(3u  << 4u);
            break;
        }

        default:
        {
            // Invalid USART
            ret = false;
            break;
        }
    }
    if (ret)
    {
        // Get USART registers
        USART_TypeDef* const usart = usart_regs[m_usart];

        // Disable receive and transmit
        usart->CR1 = 0x00u;

        // Configure transfer : parity, stop bits, flow control
        switch (m_parity)
        {
            case PARITY_NONE:
            {
                break;
            }

            case PARITY_EVEN:
            {
                usart->CR1 = (2u << 10u);
                break;
            }

            case PARITY_ODD:
            {
                usart->CR1 = (3u << 10u);
                break;
            }

            default:
            {
                // Invalid configuration
                ret = false;
                break;
            }
        }
        switch (m_stop_bits)
        {
            case STOPBITS_HALF:
            {
                usart->CR2 = (1u << 12u);
                break;
            }

            case STOPBITS_ONE:
            {
                usart->CR2 = (0u << 12u);
                break;
            }

            case STOPBITS_ONE_HALF:
            {
                usart->CR2 = (3u << 12u);
                break;
            }

            case STOPBITS_TWO:
            {
                usart->CR2 = (2u << 12u);
                break;
            }

            default:
            {
                // Invalid configuration
                ret = false;
                break;
            }
        }
        switch (m_flow_control)
        {
            case FLOWCONTROL_NONE:
            {
                usart->CR3 = 0x00u;
                break;
            }

            case FLOWCONTROL_CTSRTS:
            {
                usart->CR3 = (3u << 8u);
                break;
            }

            default:
            {
                // Invalid configuration
                ret = false;
                break;
            }
        }
        
        // Configure baudrate
        const uint32_t input_clock_freq = m_cpu.getPclk1Frequency();
        const uint32_t usart_div = input_clock_freq / m_baudrate;
        usart->BRR = usart_div;

        // Enable receive and transmit
        usart->CR1 |= (1u << 0u) | (1u << 2u) | (1u << 3u);
    }

    return ret;
}

/** \brief Write data through the UART */
bool Stm32l476Usart::write(const uint8_t* const data, const uint32_t size)
{
    uint32_t data_len = size;
    const uint8_t* current = data;
    USART_TypeDef* const usart = usart_regs[m_usart];

    while (data_len != 0)
    {
        while (((usart->ISR & (1u << 7u)) == 0)) // TXE bit
        {}

        // Send byte
        usart->TDR = (*current);

        // Next byte
        current++;
        data_len--;
    }

    return true;
}

/** \brief Read data through the UART */
bool Stm32l476Usart::read(uint8_t* const data, const uint32_t size, const uint32_t timeout)
{
    return false;
}

/** \brief IRQ handler */
void Stm32l476Usart::irqHandler()
{

}


/** \brief IRQ handler for USART 1 */
extern "C" void USART1_IRQHandler(void)
{
    usarts[Stm32l476Usart::USART_1]->irqHandler();
}

/** \brief IRQ handler for USART 2 */
extern "C" void USART2_IRQHandler(void)
{
    usarts[Stm32l476Usart::USART_2]->irqHandler();
}

/** \brief IRQ handler for USART 3 */
extern "C" void USART3_IRQHandler(void)
{
    usarts[Stm32l476Usart::USART_3]->irqHandler();
}

}
