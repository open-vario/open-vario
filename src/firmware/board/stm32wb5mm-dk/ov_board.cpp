

#include "ov_board.h"

#include "otp.h"
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
      m_oled_display(m_spi1_drv, 0u, m_oled_reset_pin, m_oled_data_pin),
      m_next_button_pin(GPIOC, GPIO_PIN_12),
      m_select_button_pin(GPIOC, GPIO_PIN_13),
      m_previous_button(),
      m_next_button(m_next_button_pin, false),
      m_select_button(m_select_button_pin, false),
      m_ble_stack()
{
}

/** @brief Initialize the clocks */
bool ov_board::clock_init()
{
    bool ret = system_clock_config();
    ret      = ret && periph_common_clock_config();
    return ret;
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

    // The OPTVERR flag is wrongly set at power on
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

    // Shutdown CPU2
    LL_C2_PWR_SetPowerMode(LL_PWR_MODE_SHUTDOWN);

    // Enable IPCC(36), HSEM(38) wakeup interrupts on CPU1
    LL_EXTI_EnableIT_32_63(LL_EXTI_LINE_36 | LL_EXTI_LINE_38);

    return ret;
}

/** @brief Initialize I/Os */
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

    // Buttons pins
    // PC12    ------> 'Previous'
    // PC13    ------> 'Select'
    gpio_init.Pin   = GPIO_PIN_12 | GPIO_PIN_13;
    gpio_init.Mode  = GPIO_MODE_INPUT;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &gpio_init);

    return ret;
}

/** @brief System Clock Configuration */
bool ov_board::system_clock_config()
{
    bool ret = true;

    RCC_OscInitTypeDef RCC_OscInitStruct = {};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
    RCC_CRSInitTypeDef RCC_CRSInitStruct = {};

    // Tick priority
    uwTickPrio = 1;

    // Disable interrupts
    __disable_irq();

    // Configure LSE Drive Capability
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    // Configure the main internal regulator output voltage
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // Configure HSE tuning from OTP values
    OTP_ID0_t* otp = reinterpret_cast<OTP_ID0_t*>(OTP_Read(0));
    LL_RCC_HSE_SetCapacitorTuning(otp->hse_tuning);

    // Initializes the RCC Oscillators according to the specified parameters
    // in the RCC_OscInitTypeDef structure.
    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI1 | RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.HSEState            = RCC_HSE_ON;
    RCC_OscInitStruct.LSEState            = RCC_LSE_ON;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_6;
    RCC_OscInitStruct.LSIState            = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN            = 32;
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2;
    RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        ret = false;
    }

    // Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK4 | RCC_CLOCKTYPE_HCLK2 | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV2;
    RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        ret = false;
    }

    // Enable MSI Auto calibration
    HAL_RCCEx_EnableMSIPLLMode();

    // Enable the SYSCFG APB clock
    __HAL_RCC_CRS_CLK_ENABLE();

    // Configures CRS
    RCC_CRSInitStruct.Prescaler             = RCC_CRS_SYNC_DIV1;
    RCC_CRSInitStruct.Source                = RCC_CRS_SYNC_SOURCE_LSE;
    RCC_CRSInitStruct.Polarity              = RCC_CRS_SYNC_POLARITY_RISING;
    RCC_CRSInitStruct.ReloadValue           = __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000, 32774);
    RCC_CRSInitStruct.ErrorLimitValue       = 34;
    RCC_CRSInitStruct.HSI48CalibrationValue = 32;

    HAL_RCCEx_CRSConfig(&RCC_CRSInitStruct);

    return ret;
}

/** @brief Peripherals Common Clock Configuration */
bool ov_board::periph_common_clock_config()
{
    bool                     ret                 = false;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {};

    // Initializes the peripherals clock
    PeriphClkInitStruct.PeriphClockSelection    = RCC_PERIPHCLK_SMPS | RCC_PERIPHCLK_RFWAKEUP | RCC_PERIPHCLK_USB | RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.PLLSAI1.PLLN            = 24;
    PeriphClkInitStruct.PLLSAI1.PLLP            = RCC_PLLP_DIV2;
    PeriphClkInitStruct.PLLSAI1.PLLQ            = RCC_PLLQ_DIV2;
    PeriphClkInitStruct.PLLSAI1.PLLR            = RCC_PLLR_DIV2;
    PeriphClkInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_USBCLK | RCC_PLLSAI1_ADCCLK;
    PeriphClkInitStruct.UsbClockSelection       = RCC_USBCLKSOURCE_PLLSAI1;
    PeriphClkInitStruct.AdcClockSelection       = RCC_ADCCLKSOURCE_PLLSAI1;
    PeriphClkInitStruct.RFWakeUpClockSelection  = RCC_RFWKPCLKSOURCE_HSE_DIV1024;
    PeriphClkInitStruct.SmpsClockSelection      = RCC_SMPSCLKSOURCE_HSI;
    PeriphClkInitStruct.SmpsDivSelection        = RCC_SMPSCLKDIV_RANGE1;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) == HAL_OK)
    {
        ret = true;
    }

    return ret;
}

} // namespace ov

/** @brief This function handles TIM1 update interrupt and TIM16 global interrupt. */
extern "C" void TIM1_UP_TIM16_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&ov::s_hal_tick);
    HAL_IncTick();
}
