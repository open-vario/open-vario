/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "stm32hal_qspi.h"

#include "os.h"
#include "thread.h"

namespace ov
{

/** @brief Address sizes */
static const uint32_t STM32_ADDRESS_SIZES[] = {QSPI_ADDRESS_8_BITS, QSPI_ADDRESS_16_BITS, QSPI_ADDRESS_24_BITS, QSPI_ADDRESS_32_BITS};

/** @brief Address modes */
static const uint32_t STM32_ADDRESS_MODES[] = {QSPI_ADDRESS_NONE, QSPI_ADDRESS_1_LINE, QSPI_ADDRESS_2_LINES, QSPI_ADDRESS_4_LINES};

/** @brief Data modes */
static const uint32_t STM32_DATA_MODES[] = {QSPI_DATA_NONE, QSPI_DATA_1_LINE, QSPI_DATA_2_LINES, QSPI_DATA_4_LINES};

/** @brief HAL QSPI handle */
static QSPI_HandleTypeDef* s_qspi;
/** @brief HAL DMA handle */
static DMA_HandleTypeDef* s_hdma_qspi;

/** @brief Constructor */
stm32hal_qspi::stm32hal_qspi() : m_qspi{}, m_hdma_qspi{}, m_rx_sem(0u, 1u), m_tx_sem(0u, 1u)
{
    // Save instances
    m_qspi.user = this;
    s_qspi      = &m_qspi;
    s_hdma_qspi = &m_hdma_qspi;
}

/** @brief Initialize the driver */
bool stm32hal_qspi::init()
{
    bool ret = false;

    // Enable clock
    __HAL_RCC_QSPI_CLK_ENABLE();
    __HAL_RCC_DMAMUX1_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    // Configure device
    m_qspi.Instance                = QUADSPI;
    m_qspi.Init.ClockPrescaler     = 2;
    m_qspi.Init.FifoThreshold      = 4;
    m_qspi.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_NONE;
    m_qspi.Init.FlashSize          = 23;
    m_qspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
    m_qspi.Init.ClockMode          = QSPI_CLOCK_MODE_0;
    if (HAL_QSPI_Init(&m_qspi) == HAL_OK)
    {
        // Enable DMA
        m_hdma_qspi.Instance                 = DMA1_Channel1;
        m_hdma_qspi.Init.Request             = DMA_REQUEST_QUADSPI;
        m_hdma_qspi.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        m_hdma_qspi.Init.PeriphInc           = DMA_PINC_DISABLE;
        m_hdma_qspi.Init.MemInc              = DMA_MINC_ENABLE;
        m_hdma_qspi.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        m_hdma_qspi.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        m_hdma_qspi.Init.Mode                = DMA_NORMAL;
        m_hdma_qspi.Init.Priority            = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&m_hdma_qspi) == HAL_OK)
        {
            // Register callbacks
            __HAL_LINKDMA(&m_qspi, hdma, m_hdma_qspi);
            m_qspi.RxCpltCallback = &stm32hal_qspi::rx_completed;
            m_qspi.TxCpltCallback = &stm32hal_qspi::tx_completed;

            // Enable interrupts
            HAL_NVIC_SetPriority(QUADSPI_IRQn, 15u, 0u);
            HAL_NVIC_EnableIRQ(QUADSPI_IRQn);
            HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 15u, 0u);
            HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
            ret = true;
        }
    }

    return ret;
}

/** @brief Execute a QSPI read command */
bool stm32hal_qspi::read(const command& cmd, void* buffer, size_t size)
{
    bool ret = false;

    QSPI_CommandTypeDef qspi_command = {};

    qspi_command.Instruction        = static_cast<uint32_t>(cmd.cmd);
    qspi_command.Address            = cmd.address;
    qspi_command.AlternateBytes     = 0;
    qspi_command.AddressSize        = STM32_ADDRESS_SIZES[static_cast<int>(cmd.addr_size)];
    qspi_command.AlternateBytesSize = 0;
    qspi_command.DummyCycles        = static_cast<uint32_t>(cmd.dummy_cycles);
    qspi_command.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    qspi_command.AddressMode        = STM32_ADDRESS_MODES[static_cast<int>(cmd.addr_mode)];
    qspi_command.AlternateByteMode  = QSPI_ALTERNATE_BYTES_NONE;
    qspi_command.DataMode           = STM32_DATA_MODES[static_cast<int>(cmd.data_mode)];
    qspi_command.NbData             = static_cast<uint32_t>(size);
    qspi_command.DdrMode            = QSPI_DDR_MODE_DISABLE;
    qspi_command.SIOOMode           = QSPI_SIOO_INST_EVERY_CMD;

    // Send the command
    if (HAL_QSPI_Command(&m_qspi, &qspi_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) == HAL_OK)
    {
        // Read data
        if (buffer && (size != 0u))
        {
            if (HAL_QSPI_Receive_DMA(&m_qspi, reinterpret_cast<uint8_t*>(buffer)) == HAL_OK)
            {
                // Wait for completion
                ret = m_rx_sem.take(HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
            }
        }
        else
        {
            ret = true;
        }
    }

    return ret;
}

/** @brief Execute a QSPI write command */
bool stm32hal_qspi::write(const command& cmd, const void* buffer, size_t size)
{
    bool ret = false;

    QSPI_CommandTypeDef qspi_command = {};

    qspi_command.Instruction        = static_cast<uint32_t>(cmd.cmd);
    qspi_command.Address            = cmd.address;
    qspi_command.AlternateBytes     = 0;
    qspi_command.AddressSize        = STM32_ADDRESS_SIZES[static_cast<int>(cmd.addr_size)];
    qspi_command.AlternateBytesSize = 0;
    qspi_command.DummyCycles        = static_cast<uint32_t>(cmd.dummy_cycles);
    qspi_command.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    qspi_command.AddressMode        = STM32_ADDRESS_MODES[static_cast<int>(cmd.addr_mode)];
    qspi_command.AlternateByteMode  = QSPI_ALTERNATE_BYTES_NONE;
    qspi_command.DataMode           = STM32_DATA_MODES[static_cast<int>(cmd.data_mode)];
    qspi_command.NbData             = static_cast<uint32_t>(size);
    qspi_command.DdrMode            = QSPI_DDR_MODE_DISABLE;
    qspi_command.SIOOMode           = QSPI_SIOO_INST_EVERY_CMD;

    // Send the command
    if (HAL_QSPI_Command(&m_qspi, &qspi_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) == HAL_OK)
    {
        // Write data
        if (buffer && (size != 0u))
        {
            if (HAL_QSPI_Transmit_DMA(&m_qspi, reinterpret_cast<uint8_t*>(const_cast<void*>(buffer))) == HAL_OK)
            {
                // Wait for completion
                ret = m_tx_sem.take(HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
            }
        }
        else
        {
            ret = true;
        }
    }

    return ret;
}

/** @brief Poll a register state */
bool stm32hal_qspi::poll(uint8_t cmd, uint8_t mask, uint8_t value, uint32_t ms_timeout)
{
    bool ret = false;

    QSPI_CommandTypeDef qspi_command = {};

    qspi_command.Instruction       = static_cast<uint32_t>(cmd);
    qspi_command.DummyCycles       = 0;
    qspi_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    qspi_command.AddressMode       = QSPI_ADDRESS_NONE;
    qspi_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    qspi_command.DataMode          = QSPI_DATA_1_LINE;
    qspi_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    qspi_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    QSPI_AutoPollingTypeDef qspi_polling;

    qspi_polling.Match           = static_cast<uint32_t>(value);
    qspi_polling.Mask            = static_cast<uint32_t>(mask);
    qspi_polling.Interval        = 0x10u;
    qspi_polling.StatusBytesSize = 1u;
    qspi_polling.MatchMode       = QSPI_MATCH_MODE_AND;
    qspi_polling.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

    if (HAL_QSPI_AutoPolling(&m_qspi, &qspi_command, &qspi_polling, ms_timeout) == HAL_OK)
    {
        ret = true;
    }

    return ret;
}

/** @brief  Rx Transfer completed callback */
void stm32hal_qspi::rx_completed(QSPI_HandleTypeDef* handle)
{
    bool           higher_priority_task_woken = false;
    stm32hal_qspi* qspi                       = reinterpret_cast<stm32hal_qspi*>(handle->user);
    qspi->m_rx_sem.release_from_isr(higher_priority_task_woken);
    ov::os::yield_from_isr(higher_priority_task_woken);
}

/** @brief  Tx completed callback */
void stm32hal_qspi::tx_completed(QSPI_HandleTypeDef* handle)
{
    bool           higher_priority_task_woken = false;
    stm32hal_qspi* qspi                       = reinterpret_cast<stm32hal_qspi*>(handle->user);
    qspi->m_tx_sem.release_from_isr(higher_priority_task_woken);
    ov::os::yield_from_isr(higher_priority_task_woken);
}

} // namespace ov

/** @brief This function handles DMA1 channel1 global interrupt */
extern "C" void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(ov::s_hdma_qspi);
}

/** @brief This function handles QUADSPI global interrupt */
extern "C" void QUADSPI_IRQHandler(void)
{
    HAL_QSPI_IRQHandler(ov::s_qspi);
}