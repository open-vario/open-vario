/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "stm32hal_lpuart.h"

#include "os.h"

#include <cstring>

namespace ov
{

/** @brief HAL USART handle */
static UART_HandleTypeDef* s_lpuart;

/** @brief Constructor */
stm32hal_lpuart::stm32hal_lpuart()
    : m_lpuart{},
      m_rx_sem(0u, 1u),
      m_tx_sem(0u, 1u),
      m_rx_byte(0),
      m_rx_buffer(),
      m_rx_write_index(0u),
      m_rx_read_index(0u),
      m_rx_bytes_count(0u)
{
    // Save instances
    m_lpuart.user = this;
    s_lpuart      = &m_lpuart;
}

/** @brief Initialize the driver */
bool stm32hal_lpuart::init()
{
    bool ret = false;

    // Enable clock
    RCC_PeriphCLKInitTypeDef clk_init = {};
    clk_init.PeriphClockSelection     = RCC_PERIPHCLK_LPUART1;
    clk_init.Lpuart1ClockSelection    = RCC_LPUART1CLKSOURCE_LSE;
    HAL_RCCEx_PeriphCLKConfig(&clk_init);
    __HAL_RCC_LPUART1_CLK_ENABLE();

    // Configure device
    m_lpuart.Instance                    = LPUART1;
    m_lpuart.Init.BaudRate               = 9600u;
    m_lpuart.Init.WordLength             = UART_WORDLENGTH_8B;
    m_lpuart.Init.StopBits               = UART_STOPBITS_1;
    m_lpuart.Init.Parity                 = UART_PARITY_NONE;
    m_lpuart.Init.Mode                   = UART_MODE_TX_RX;
    m_lpuart.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    m_lpuart.Init.OverSampling           = UART_OVERSAMPLING_16;
    m_lpuart.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    m_lpuart.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    m_lpuart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&m_lpuart) == HAL_OK)
    {
        // Disable FIFOs
        HAL_UARTEx_SetTxFifoThreshold(&m_lpuart, UART_TXFIFO_THRESHOLD_1_8);
        HAL_UARTEx_SetRxFifoThreshold(&m_lpuart, UART_RXFIFO_THRESHOLD_1_8);
        HAL_UARTEx_DisableFifoMode(&m_lpuart);

        // Register callbacks
        m_lpuart.RxCpltCallback = &stm32hal_lpuart::rx_completed;
        m_lpuart.TxCpltCallback = &stm32hal_lpuart::tx_completed;

        // Start reception using interrupts
        if (HAL_UART_Receive_IT(&m_lpuart, &m_rx_byte, 1u) == HAL_OK)
        {
            // Enable interrupts
            HAL_NVIC_SetPriority(LPUART1_IRQn, 15u, 0u);
            HAL_NVIC_EnableIRQ(LPUART1_IRQn);
        }

        ret = true;
    }

    return ret;
}

/** @brief Read data from the serial port */
bool stm32hal_lpuart::read(void* buffer, size_t size, uint32_t ms_timeout)
{
    bool ret = true;

    // Read loop
    size_t   read_count = 0;
    uint8_t* u8_buffer  = reinterpret_cast<uint8_t*>(buffer);
    while (ret && (read_count != size))
    {
        // Wait for incoming data
        if (m_rx_bytes_count == 0u)
        {
            ret = m_rx_sem.take(ms_timeout);
        }
        if (ret)
        {
            // Disable RX interrupts
            ATOMIC_CLEAR_BIT(m_lpuart.Instance->CR1, USART_CR1_RXNEIE_RXFNEIE);

            // Read available bytes
            while ((m_rx_bytes_count != 0u) && (read_count != size))
            {
                *u8_buffer = m_rx_buffer[m_rx_read_index];
                u8_buffer++;
                m_rx_read_index++;
                read_count++;
                m_rx_bytes_count--;
                if (m_rx_read_index == RX_BUFFER_SIZE)
                {
                    m_rx_read_index = 0;
                }
            }

            // Enable RX interrupts
            ATOMIC_SET_BIT(m_lpuart.Instance->CR1, USART_CR1_RXNEIE_RXFNEIE);
        }
    }

    return ret;
}

/** @brief Write data to the serial port */
bool stm32hal_lpuart::write(const void* buffer, size_t size)
{

    bool ret = true;

    // Start transmission using interrupts
    if (size != 0)
    {
        if (HAL_UART_Transmit_IT(&m_lpuart, reinterpret_cast<const uint8_t*>(buffer), static_cast<size_t>(size)) == HAL_OK)
        {
            // Wait end of transmission
            m_tx_sem.take();
            ret = true;
        }
    }
    else
    {
        ret = true;
    }

    return ret;
}

/** @brief Write a null-terminated string to the serial port */
bool stm32hal_lpuart::write(const char* str)
{
    return write(str, strlen(str));
}

/** @brief  Rx Transfer completed callback */
void stm32hal_lpuart::rx_completed(UART_HandleTypeDef* handle)
{
    bool             higher_priority_task_woken = false;
    stm32hal_lpuart* usart                      = reinterpret_cast<stm32hal_lpuart*>(handle->user);

    // Store received byte
    if (usart->m_rx_bytes_count != stm32hal_lpuart::RX_BUFFER_SIZE)
    {
        usart->m_rx_buffer[usart->m_rx_write_index] = usart->m_rx_byte;
        usart->m_rx_write_index++;
        usart->m_rx_bytes_count++;
        if (usart->m_rx_write_index == stm32hal_lpuart::RX_BUFFER_SIZE)
        {
            usart->m_rx_write_index = 0;
        }

        // Notify reception
        usart->m_rx_sem.release_from_isr(higher_priority_task_woken);
        ov::os::yield_from_isr(higher_priority_task_woken);
    }

    // Restart reception
    HAL_UART_Receive_IT(&usart->m_lpuart, &usart->m_rx_byte, 1u);
}

/** @brief  Tx completed callback */
void stm32hal_lpuart::tx_completed(UART_HandleTypeDef* handle)
{
    bool             higher_priority_task_woken = false;
    stm32hal_lpuart* usart                      = reinterpret_cast<stm32hal_lpuart*>(handle->user);
    usart->m_tx_sem.release_from_isr(higher_priority_task_woken);
    ov::os::yield_from_isr(higher_priority_task_woken);
}

} // namespace ov

/** @brief This function handles LPUART1 interrupt */
extern "C" void LPUART1_IRQHandler()
{
    HAL_UART_IRQHandler(ov::s_lpuart);
}
