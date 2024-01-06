

#include "stm32hal_usart.h"

#include "os.h"

#include <cstring>

namespace ov
{

/** @brief HAL USART handle */
static UART_HandleTypeDef* s_usart;

/** @brief Constructor */
stm32hal_usart::stm32hal_usart()
    : m_usart{},
      m_rx_sem(0u, 1u),
      m_tx_sem(0u, 1u),
      m_rx_byte(0),
      m_rx_buffer(),
      m_rx_write_index(0u),
      m_rx_read_index(0u),
      m_rx_bytes_count(0u)
{
    // Save instances
    m_usart.user = this;
    s_usart      = &m_usart;
}

/** @brief Initialize the driver */
bool stm32hal_usart::init()
{
    bool ret = false;

    // Enable clock
    RCC_PeriphCLKInitTypeDef clk_init = {};
    clk_init.PeriphClockSelection     = RCC_PERIPHCLK_USART1;
    clk_init.Usart1ClockSelection     = RCC_USART1CLKSOURCE_PCLK2;
    HAL_RCCEx_PeriphCLKConfig(&clk_init);
    __HAL_RCC_USART1_CLK_ENABLE();

    // Configure device
    m_usart.Instance                    = USART1;
    m_usart.Init.BaudRate               = 115200u;
    m_usart.Init.WordLength             = UART_WORDLENGTH_8B;
    m_usart.Init.StopBits               = UART_STOPBITS_1;
    m_usart.Init.Parity                 = UART_PARITY_NONE;
    m_usart.Init.Mode                   = UART_MODE_TX_RX;
    m_usart.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    m_usart.Init.OverSampling           = UART_OVERSAMPLING_16;
    m_usart.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    m_usart.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    m_usart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&m_usart) == HAL_OK)
    {
        // Disable FIFOs
        HAL_UARTEx_SetTxFifoThreshold(&m_usart, UART_TXFIFO_THRESHOLD_1_8);
        HAL_UARTEx_SetRxFifoThreshold(&m_usart, UART_RXFIFO_THRESHOLD_1_8);
        HAL_UARTEx_DisableFifoMode(&m_usart);

        // Register callbacks
        m_usart.RxCpltCallback = &stm32hal_usart::rx_completed;
        m_usart.TxCpltCallback = &stm32hal_usart::tx_completed;

        // Start reception using interrupts
        if (HAL_UART_Receive_IT(&m_usart, &m_rx_byte, 1u) == HAL_OK)
        {
            // Enable interrupts
            HAL_NVIC_SetPriority(USART1_IRQn, 15u, 0u);
            HAL_NVIC_EnableIRQ(USART1_IRQn);
        }

        ret = true;
    }

    return ret;
}

/** @brief Read data from the serial port */
bool stm32hal_usart::read(void* buffer, size_t size, uint32_t ms_timeout)
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
            ATOMIC_CLEAR_BIT(m_usart.Instance->CR1, USART_CR1_RXNEIE_RXFNEIE);

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
            ATOMIC_SET_BIT(m_usart.Instance->CR1, USART_CR1_RXNEIE_RXFNEIE);
        }
    }

    return ret;
}

/** @brief Write data to the serial port */
bool stm32hal_usart::write(const void* buffer, size_t size)
{

    bool ret = true;

    // Start transmission using interrupts
    if (size != 0)
    {
        if (HAL_UART_Transmit_IT(&m_usart, reinterpret_cast<const uint8_t*>(buffer), static_cast<size_t>(size)) == HAL_OK)
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
bool stm32hal_usart::write(const char* str)
{
    return write(str, strlen(str));
}

/** @brief  Rx Transfer completed callback */
void stm32hal_usart::rx_completed(UART_HandleTypeDef* handle)
{
    bool            higher_priority_task_woken = false;
    stm32hal_usart* usart                      = reinterpret_cast<stm32hal_usart*>(handle->user);

    // Store received byte
    if (usart->m_rx_bytes_count != stm32hal_usart::RX_BUFFER_SIZE)
    {
        usart->m_rx_buffer[usart->m_rx_write_index] = usart->m_rx_byte;
        usart->m_rx_write_index++;
        usart->m_rx_bytes_count++;
        if (usart->m_rx_write_index == stm32hal_usart::RX_BUFFER_SIZE)
        {
            usart->m_rx_write_index = 0;
        }

        // Notify reception
        usart->m_rx_sem.release_from_isr(higher_priority_task_woken);
        ov::os::yield_from_isr(higher_priority_task_woken);
    }

    // Restart reception
    HAL_UART_Receive_IT(&usart->m_usart, &usart->m_rx_byte, 1u);
}

/** @brief  Tx completed callback */
void stm32hal_usart::tx_completed(UART_HandleTypeDef* handle)
{
    bool            higher_priority_task_woken = false;
    stm32hal_usart* usart                      = reinterpret_cast<stm32hal_usart*>(handle->user);
    usart->m_tx_sem.release_from_isr(higher_priority_task_woken);
    ov::os::yield_from_isr(higher_priority_task_woken);
}

} // namespace ov

/** @brief This function handles USART1 interrupt */
extern "C" void USART1_IRQHandler()
{
    HAL_UART_IRQHandler(ov::s_usart);
}
