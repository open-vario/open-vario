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

#include "Stm32l476LpUart.h"
#include "Stm32l476Cpu.h"
#include "IOs.h"

#include "stm32l476xx.h"

#include <cstddef>

namespace open_vario
{

/** \brief Rx ready flag mask */
static const uint32_t UART_RX_READY_FLAG_MASK = 1u;

/** \brief Tx ready flag mask */
static const uint32_t UART_TX_READY_FLAG_MASK = 2u;



/** \brief LPUART instance */
static Stm32l476LpUart* lpuart = nullptr;



/** \brief Constructor */
Stm32l476LpUart::Stm32l476LpUart(const Stm32l476Cpu& cpu, const uint32_t baudrate, const Parity parity, const StopBits stop_bits, const FlowControl flow_control)
: m_cpu(cpu)
, m_baudrate(baudrate)
, m_parity(parity)
, m_stop_bits(stop_bits)
, m_flow_control(flow_control)
, m_status_flags(0u)
, m_rx_queue()
, m_tx_queue()
{
    // Register instance
    lpuart = this;
}

/** \brief Configure the UART */
bool Stm32l476LpUart::configure()
{
    bool ret = true;

    // Enable clock and configure to use PCLK as input clock for baudrates > 9600 and LSE for slower baudrates */
    uint32_t input_clock_freq;
    RCC->APB1ENR2 |= (1u << 0u);
    if (m_baudrate > 9600)
    {
        RCC->CCIPR &= ~(3u  << 10u);
        input_clock_freq = m_cpu.getPclk1Frequency();
    }
    else
    {
        RCC->CCIPR |= (3u  << 10u);
        input_clock_freq = m_cpu.getLseFrequency();
    }
    
    // Disable receive and transmit
    LPUART1->CR1 = 0x00u;

    // Configure transfer : parity, stop bits, flow control
    switch (m_parity)
    {
        case PARITY_NONE:
        {
            break;
        }

        case PARITY_EVEN:
        {
            LPUART1->CR1 = (2u << 10u);
            break;
        }

        case PARITY_ODD:
        {
            LPUART1->CR1 = (3u << 10u);
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
        case STOPBITS_ONE:
        {
            LPUART1->CR2 = (0u << 12u);
            break;
        }

        case STOPBITS_TWO:
        {
            LPUART1->CR2 = (2u << 12u);
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
            LPUART1->CR3 = 0x00u;
            break;
        }

        case FLOWCONTROL_CTSRTS:
        {
            LPUART1->CR3 = (3u << 8u);
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
    const uint32_t usart_div = 256u * input_clock_freq / m_baudrate;
    LPUART1->BRR = usart_div;

    // Enable Rx interrupt
    LPUART1->CR1 |= (1u << 5u);
    NVIC_EnableIRQ(LPUART1_IRQn);

    // Enable receive and transmit
    LPUART1->CR1 |= (1u << 0u) | (1u << 2u) | (1u << 3u);

    return ret;
}

/** \brief Write data through the UART */
bool Stm32l476LpUart::write(const uint8_t* const data, const uint32_t size)
{
    bool ret = true;
    uint32_t data_left = size;
    const uint8_t* current = data;

    while (ret && (data_left != 0u))
    {
        // Disable Rx and Tx interrupts
        LPUART1->CR1 &= ~((1u << 5u) | (1u << 7u));

        // Add data to the transmit buffer
        while ((data_left != 0u) && m_tx_queue.push(*current))
        {
            current++;
            data_left--;
        }
        if (data_left != 0u)
        {
            m_status_flags.reset(UART_TX_READY_FLAG_MASK);
        }

        // Enable Rx and Tx interrupts
        LPUART1->CR1 |= ((1u << 5u) | (1u << 7u));

        // Wait for Tx ready if more data has to be sent
        if (data_left != 0u)
        {
            uint32_t wake_up_flags = UART_TX_READY_FLAG_MASK;
            ret = m_status_flags.wait(wake_up_flags, false, IOs::getInstance().getInfiniteTimeoutValue());
        }
    }

    return ret;
}

/** \brief Read data through the UART */
bool Stm32l476LpUart::read(uint8_t* const data, const uint32_t size, const uint32_t timeout)
{
    bool ret = true;
    uint8_t* current = data;
    uint32_t data_left = size;

    while (ret && (data_left != 0))
    {
        // Save interrupt mask
        uint32_t int_mask = LPUART1->CR1;

        // Disable Rx and Tx interrupts
        LPUART1->CR1 &= ~((1u << 5u) | (1u << 7u));

        // Get received data
        while ((data_left != 0u) && m_rx_queue.pop(*current))
        {
            current++;
            data_left--;
        }
        if (m_rx_queue.getCount() == 0u)
        {
            m_status_flags.reset(UART_RX_READY_FLAG_MASK);
        }

        // Restore interrupt mask
        LPUART1->CR1 = int_mask;

        // Wait for Rx ready if more data has to be received
        if (data_left != 0)
        {
            uint32_t wake_up_flags = UART_RX_READY_FLAG_MASK;
            ret = m_status_flags.wait(wake_up_flags, false, timeout);
        }
    }

    return ret;
}

/** \brief IRQ handler */
void Stm32l476LpUart::irqHandler()
{
    #ifdef OS_NANO_OS
    NANO_OS_INTERRUPT_Enter();
    #endif // OS_NANO_OS

    bool signal;
    static volatile USART_TypeDef* lpuart_regs = LPUART1; 

    // Read received data
    signal = (m_rx_queue.getCount() == 0u);
    while ((LPUART1->ISR & (1u << 5u)) != 0) // RXNE bit
    {
        // Read the received byte
        m_rx_queue.push(static_cast<uint8_t>(LPUART1->RDR));
    }

    // Signal received data
    if (signal && (m_rx_queue.getCount() != 0u))
    {
        m_status_flags.set(UART_RX_READY_FLAG_MASK, true);
    }

    // Transmit data
    if ((LPUART1->ISR & (1u << 7u)) != 0) // TXE bit
    {
		if (m_tx_queue.getCount() != 0u)
		{
		    // Check if Tx queue was full
		    signal = (m_tx_queue.getCount() == m_tx_queue.getCapacity());

            // Send byte
            uint8_t data;
            m_tx_queue.pop(data);
			LPUART1->TDR = static_cast<uint16_t>(data);

			// Check end of transmit */
			if (m_tx_queue.getCount() == 0u)
			{
				// Disable TXE interrupt
			    LPUART1->CR1 &= ~(1u << 7u);
			}

			// Signal Tx ready
			if (signal)
			{
                m_status_flags.set(UART_TX_READY_FLAG_MASK, true);
			}
		}
		else
		{
		    // Disable TXE interrupt
		    LPUART1->CR1 &= ~(1u << 7u);
		}
    }

    // Clear error flags
    LPUART1->ICR = 0xFFFFFFFFu;
    lpuart_regs->ICR = 0;

    #ifdef OS_NANO_OS
    NANO_OS_INTERRUPT_Exit();
    #endif // OS_NANO_OS
}


/** \brief IRQ handler for LPUART 1 */
extern "C" void LPUART1_IRQHandler(void)
{
    lpuart->irqHandler();
}


}
