
#include "stm32hal_spi.h"
#include "os.h"

namespace ov
{

/** @brief Polarities */
static const uint32_t STM32_POLARITY[] = {SPI_POLARITY_LOW, SPI_POLARITY_HIGH};

/** @brief Phases */
static const uint32_t STM32_PHASE[] = {SPI_PHASE_1EDGE, SPI_PHASE_2EDGE};

/** @brief HAL SPI handles */
static SPI_HandleTypeDef* s_spis[2u];

/** @brief Constructor */
stm32hal_spi::stm32hal_spi(SPI_TypeDef* instance, uint32_t baudrate, polarity pol, phase pha, i_cs_driver& cs_driver)
    : m_cs_driver(cs_driver), m_spi{}, m_baudrate(baudrate), m_xfer_sem(0u, 1u)
{
    // Save parameters
    m_spi.Instance         = instance;
    m_spi.Init.CLKPolarity = STM32_POLARITY[static_cast<int>(pol)];
    m_spi.Init.CLKPhase    = STM32_PHASE[static_cast<int>(pha)];
    m_spi.user             = this;
    if (m_spi.Instance == SPI1)
    {
        s_spis[0u] = &m_spi;
    }
    else
    {
        s_spis[1u] = &m_spi;
    }
}

/** @brief Initialize the driver */
bool stm32hal_spi::init()
{
    bool ret = false;

    // Enable clock
    if (m_spi.Instance == SPI1)
    {
        __HAL_RCC_SPI1_CLK_ENABLE();
    }
    else
    {
        __HAL_RCC_SPI2_CLK_ENABLE();
    }

    // Configure device
    m_spi.Init.Mode              = SPI_MODE_MASTER;
    m_spi.Init.Direction         = SPI_DIRECTION_2LINES;
    m_spi.Init.DataSize          = SPI_DATASIZE_8BIT;
    m_spi.Init.NSS               = SPI_NSS_SOFT;
    m_spi.Init.BaudRatePrescaler = compute_prescaler();
    m_spi.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    m_spi.Init.TIMode            = SPI_TIMODE_DISABLE;
    m_spi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    if (HAL_SPI_Init(&m_spi) == HAL_OK)
    {
        // Register callbacks
        m_spi.RxCpltCallback = &stm32hal_spi::rx_completed;
        m_spi.TxCpltCallback = &stm32hal_spi::tx_completed;

        // Enable interrupts
        if (m_spi.Instance == SPI1)
        {
            HAL_NVIC_SetPriority(SPI1_IRQn, 15u, 0u);
            HAL_NVIC_EnableIRQ(SPI1_IRQn);
        }
        else
        {
            HAL_NVIC_SetPriority(SPI2_IRQn, 15u, 0u);
            HAL_NVIC_EnableIRQ(SPI2_IRQn);
        }

        ret = true;
    }

    return ret;
}

/** @brief Transfer data through the SPI */
bool stm32hal_spi::xfer(const xfer_desc& xfer)
{
    bool ret = false;

    // Transfer loop
    const xfer_desc* current_xfer = &xfer;
    do
    {
        // Enable peripheral selection
        m_cs_driver.enable(current_xfer->cs);

        // Execute transfer
        HAL_StatusTypeDef err;
        if (current_xfer->read_data && current_xfer->write_data)
        {
            err = HAL_SPI_TransmitReceive(
                &m_spi, const_cast<uint8_t*>(current_xfer->write_data), current_xfer->read_data, current_xfer->size, 10000u);
        }
        else if (current_xfer->read_data)
        {
            err = HAL_SPI_Receive(&m_spi, current_xfer->read_data, current_xfer->size, 10000u);
        }
        else
        {
            err = HAL_SPI_Transmit(&m_spi, const_cast<uint8_t*>(current_xfer->write_data), current_xfer->size, 10000u);
        }
        ret = (err == HAL_OK);

        // Release peripheral selection
        if (!current_xfer->keep_cs_active)
        {
            m_cs_driver.disable(current_xfer->cs);
        }

        // Next transfer
        current_xfer = current_xfer->next;

    } while (current_xfer && ret);

    return ret;
}

/** @brief Compute the prescaler value */
uint32_t stm32hal_spi::compute_prescaler()
{
    uint32_t index         = 0;
    uint32_t prescaler     = 0;
    uint32_t spi_input_clk = 0;

    static const uint32_t baudfreq[] = {
        SPI_BAUDRATEPRESCALER_2,
        SPI_BAUDRATEPRESCALER_4,
        SPI_BAUDRATEPRESCALER_8,
        SPI_BAUDRATEPRESCALER_16,
        SPI_BAUDRATEPRESCALER_32,
        SPI_BAUDRATEPRESCALER_64,
        SPI_BAUDRATEPRESCALER_128,
        SPI_BAUDRATEPRESCALER_256,
    };

    if (m_spi.Instance == SPI1)
    {
        spi_input_clk = HAL_RCC_GetPCLK2Freq();
    }
    else
    {
        spi_input_clk = HAL_RCC_GetPCLK1Freq();
    }

    while (spi_input_clk > m_baudrate)
    {
        prescaler     = baudfreq[index];
        spi_input_clk = (spi_input_clk >> 1);
        index++;
        if (index > 7u)
        {
            break;
        }
    }

    return prescaler;
}

/** @brief  Rx completed callback */
void stm32hal_spi::rx_completed(SPI_HandleTypeDef* handle)
{
    bool          higher_priority_task_woken = false;
    stm32hal_spi* spi                        = reinterpret_cast<stm32hal_spi*>(handle->user);
    spi->m_xfer_sem.release_from_isr(higher_priority_task_woken);
    ov::os::yield_from_isr(higher_priority_task_woken);
}

/** @brief  Tx completed callback */
void stm32hal_spi::tx_completed(SPI_HandleTypeDef* handle)
{
    bool          higher_priority_task_woken = false;
    stm32hal_spi* spi                        = reinterpret_cast<stm32hal_spi*>(handle->user);
    spi->m_xfer_sem.release_from_isr(higher_priority_task_woken);
    ov::os::yield_from_isr(higher_priority_task_woken);
}

} // namespace ov

/** @brief This function handles SPI1 global interrupt */
extern "C" void SPI1_IRQHandler(void)
{
    HAL_SPI_IRQHandler(ov::s_spis[0u]);
}

/** @brief This function handles SPI2 global interrupt */
extern "C" void SPI2_IRQHandler(void)
{
    HAL_SPI_IRQHandler(ov::s_spis[1u]);
}
