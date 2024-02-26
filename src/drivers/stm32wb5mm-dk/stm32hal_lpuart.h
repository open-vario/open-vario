/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_STM32HAL_LPUART_H
#define OV_STM32HAL_LPUART_H

#include "i_serial.h"
#include "semaphore.h"

#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_uart.h"

namespace ov
{

/** @brief Serial port driver implementation using STM32HAL and LPUART peripheral */
class stm32hal_lpuart : public i_serial
{
  public:
    /** @brief Constructor */
    stm32hal_lpuart();

    /** @brief Initialize the driver */
    bool init();

    /** @brief Read data from the serial port */
    bool read(void* buffer, size_t size, uint32_t ms_timeout) override;

    /** @brief Write data to the serial port */
    bool write(const void* buffer, size_t size) override;

    /** @brief Write a null-terminated string to the serial port */
    bool write(const char* str) override;

  private:
    /** @brief Size of the rx buffer in bytes */
    static constexpr uint32_t RX_BUFFER_SIZE = 512u;

    /** @brief HAL USART handle */
    UART_HandleTypeDef m_lpuart;
    /** @brief Rx semaphore */
    semaphore m_rx_sem;
    /** @brief Tx semaphore */
    semaphore m_tx_sem;
    /** @brief Rx byte */
    uint8_t m_rx_byte;
    /** @brief Rx buffer */
    uint8_t m_rx_buffer[RX_BUFFER_SIZE];
    /** @brief Current rx write index */
    uint32_t m_rx_write_index;
    /** @brief Current rx read index */
    uint32_t m_rx_read_index;
    /** @brief Number of bytes in the rx buffer */
    uint32_t m_rx_bytes_count;

    /** @brief  Rx completed callback */
    static void rx_completed(UART_HandleTypeDef* handle);
    /** @brief  Tx completed callback */
    static void tx_completed(UART_HandleTypeDef* handle);
};

} // namespace ov

#endif // OV_STM32HAL_LPUART_H
