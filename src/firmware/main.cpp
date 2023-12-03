
#include "stm32wbxx_hal.h"

#include "delegate.h"
#include "lock_guard.h"
#include "mutex.h"
#include "os.h"
#include "semaphore.h"
#include "thread.h"

/** @brief System Clock Configuration */
static void SystemClock_Config(void);
/** @brief Peripherals Common Clock Configuration */
static void PeriphCommonClock_Config(void);

class Test
{
  public:
    Test() { }

    void   f1() { count += 1; }
    void   f2(size_t val) { count += val; }
    size_t f3(void)
    {
        count += 1;
        return count;
    }
    size_t f4(size_t val)
    {
        count += val;
        return count;
    }
    size_t f5(size_t val, bool add)
    {
        count += val;
        if (add)
        {
            count += val;
        }
        return count;
    }

    size_t f6(const size_t& val)
    {
        count += val;
        return count;
    }

    void start()
    {
        auto start_func = ov::thread_func::create<Test, &Test::task_func>(*this);
        thread.start(start_func, "Test", 3u, this);
    }

  private:
    ov::thread<4096u> thread;
    ov::mutex         mutex;
    size_t            count = 0;

    void task_func(void* param)
    {
        Test* test = reinterpret_cast<Test*>(param);
        while (test == this)
        {
            ov::lock_guard<ov::mutex> lock(test->mutex);
            test->f1();
            ov::this_thread::sleep_for(2000u);
        }
    }
};

static Test t;

/** @brief Entry point */
int main()
{
    ov::mutex mutex;
    {
        ov::lock_guard<ov::mutex> lock(mutex);
    }

    ov::semaphore sem(0, 10u);
    sem.release();
    sem.take();

    auto delegate1 = ov::delegate<void>::create<Test, &Test::f1>(t);
    delegate1.invoke();
    auto delegate2 = ov::delegate<void, size_t>::create<Test, &Test::f2>(t);
    delegate2.invoke(1234u);
    auto delegate3 = ov::delegate<size_t>::create<Test, &Test::f3>(t);
    delegate3.invoke();
    auto delegate4 = ov::delegate<size_t, size_t>::create<Test, &Test::f4>(t);
    delegate4.invoke(1234u);
    auto delegate5 = ov::delegate<size_t, size_t, bool>::create<Test, &Test::f5>(t);
    delegate5.invoke(1234u, true);
    size_t n         = 100000u;
    auto   delegate6 = ov::delegate<size_t, const size_t&>::create<Test, &Test::f6>(t);
    delegate6.invoke(n);

    t.start();

    // Configure clocks
    SystemClock_Config();
    PeriphCommonClock_Config();

    // Start operating system
    ov::os::start();

    // Shall never happen
    return 0;
}

/** @brief System Clock Configuration */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
    RCC_CRSInitTypeDef RCC_CRSInitStruct = {};

    // Tick priority
    uwTickPrio = 1;

    // Configure LSE Drive Capability
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    // Configure the main internal regulator output voltage
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

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
        while (true)
        {
        }
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
        while (true)
        {
        }
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
}

/** @brief Peripherals Common Clock Configuration */
void PeriphCommonClock_Config(void)
{
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

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        while (true)
        {
        }
    }
}
