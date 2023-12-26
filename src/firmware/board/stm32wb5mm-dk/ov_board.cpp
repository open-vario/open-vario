

#include "ov_board.h"

#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_gpio.h"
#include "stm32wbxx_hal_tim.h"

namespace ov
{

/** @brief HAL timer handle for the tick */
static TIM_HandleTypeDef s_hal_tick;

/** @brief Constructor */
ov_board::ov_board()
    : m_dbg_usart_drv(),
      m_qspi_drv(),
      m_oled_cs_pin(GPIOH, GPIO_PIN_0),
      m_spi1_cs_drv({&m_oled_cs_pin}),
      m_spi1_drv(SPI1, 10000000u, i_spi::polarity::high, i_spi::phase::first, m_spi1_cs_drv),
      m_qspi_nor_flash(s25flxxxs::ref::s25fl128s, m_qspi_drv),
      m_oled_reset_pin(GPIOC, GPIO_PIN_8),
      m_oled_data_pin(GPIOC, GPIO_PIN_9),
      m_oled_display(m_spi1_drv, 0u, m_oled_reset_pin, m_oled_data_pin)
{
}

/** @brief Initialize the peripherals */
bool ov_board::init()
{
    bool ret;

    // Initialize drivers
    ret = hal_init();
    ret = ret && io_init();
    ret = ret && m_dbg_usart_drv.init();
    ret = ret && m_qspi_drv.init();
    ret = ret && m_spi1_cs_drv.init();
    ret = ret && m_spi1_drv.init();

    // Initialize peripherals
    ret = ret && m_qspi_nor_flash.reset();
    ret = ret && m_oled_display.init();

    return ret;
}

/** @brief Initialize the HAL */
bool ov_board::hal_init()
{
    bool ret = false;

    // Use TIMER16 for HAL tick
    // => 1ms period

    // Enable tick clock
    __HAL_RCC_TIM16_CLK_ENABLE();

    // Get tick input frequency
    uint32_t tick_input_freq = HAL_RCC_GetPCLK2Freq();

    // Initialize tick peripheral as follow:
    // + Period = [(TIM16CLK/1000) - 1]. to have a (1/1000) s time base.
    // + Prescaler = (tick_input_freq/1000000 - 1) to have a 1MHz counter clock.
    // + ClockDivision = 0
    // + Counter direction = Up
    uint32_t tick_prescaler = static_cast<uint32_t>((tick_input_freq / 1000000u) - 1u);

    // Configure timer
    s_hal_tick.Instance           = TIM16;
    s_hal_tick.Init.Period        = (1000000U / 1000U) - 1U;
    s_hal_tick.Init.Prescaler     = tick_prescaler;
    s_hal_tick.Init.ClockDivision = 0;
    s_hal_tick.Init.CounterMode   = TIM_COUNTERMODE_UP;
    if (HAL_TIM_Base_Init(&s_hal_tick) == HAL_OK)
    {
        /* Start the TIM time Base generation in interrupt mode */
        if (HAL_TIM_Base_Start_IT(&s_hal_tick) == HAL_OK)
        {
            ret = true;
        }
    }

    // Configure tick interrupt
    HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 15u, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);

    return ret;
}

/** @brief Initialize I/Os*/
bool ov_board::io_init()
{
    bool ret = true;

    GPIO_InitTypeDef gpio_init;

    // Enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    // UART1(debug) pins
    // PB7     ------> USART1_RX
    // PB6     ------> USART1_TX
    gpio_init.Pin       = GPIO_PIN_7 | GPIO_PIN_6;
    gpio_init.Mode      = GPIO_MODE_AF_PP;
    gpio_init.Pull      = GPIO_NOPULL;
    gpio_init.Speed     = GPIO_SPEED_FREQ_MEDIUM;
    gpio_init.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &gpio_init);

    // QSPI pins
    // PB9     ------> QUADSPI_BK1_IO0
    // PA3     ------> QUADSPI_CLK
    // PD7     ------> QUADSPI_BK1_IO3
    // PD3     ------> QUADSPI_BK1_NCS
    // PD5     ------> QUADSPI_BK1_IO1
    // PD6     ------> QUADSPI_BK1_IO2
    gpio_init.Pin       = GPIO_PIN_9;
    gpio_init.Mode      = GPIO_MODE_AF_PP;
    gpio_init.Pull      = GPIO_NOPULL;
    gpio_init.Speed     = GPIO_SPEED_FREQ_LOW;
    gpio_init.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOB, &gpio_init);

    gpio_init.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &gpio_init);

    gpio_init.Pin   = GPIO_PIN_7 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_6;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &gpio_init);

    // SPI1 pins
    // PA1     ------> SPI1_SCK
    // PA7     ------> SPI1_MOSI
    // PB4     ------> SPI1_MISO
    gpio_init.Pin       = GPIO_PIN_1 | GPIO_PIN_7;
    gpio_init.Mode      = GPIO_MODE_AF_PP;
    gpio_init.Pull      = GPIO_PULLDOWN;
    gpio_init.Speed     = GPIO_SPEED_FREQ_LOW;
    gpio_init.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &gpio_init);

    gpio_init.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOB, &gpio_init);

    // OLED driver pins
    // PC8     ------> Reset
    // PC9     ------> Data/Control
    // PH0     ------> SPI chip select
    gpio_init.Pin   = GPIO_PIN_8 | GPIO_PIN_9;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &gpio_init);

    gpio_init.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOH, &gpio_init);

    return ret;
}

} // namespace ov

/** @brief This function handles TIM1 update interrupt and TIM16 global interrupt. */
extern "C" void TIM1_UP_TIM16_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&ov::s_hal_tick);
    HAL_IncTick();
}
