
#include "stm32wbxx_hal.h"

#include "fs.h"
#include "os.h"
#include "ov_board.h"
#include "thread.h"

/** @brief System Clock Configuration */
static void SystemClock_Config(void);
/** @brief Peripherals Common Clock Configuration */
static void PeriphCommonClock_Config(void);

class Test
{
  public:
    Test() { }

    void start()
    {
        auto start_func = ov::thread_func::create<Test, &Test::task_func>(*this);
        m_thread.start(start_func, "Test", 3u, this);
    }

  private:
    ov::ov_board      m_board;
    ov::thread<4096u> m_thread;

    void task_func(void*)
    {
        // Init board
        m_board.init();

        // Init filesystem
        bool fs_reinitialized = false;
        ov::fs::init(fs_reinitialized, m_board.get_storage_memory());

        size_t free_space = 0;
        size_t total_size = 0;
        ov::fs::info(free_space, total_size);

        {
            ov::fs::mkdir("/test");

            ov::file f = ov::fs::open("/test.dat", ov::fs::o_creat | ov::fs::o_rdwr);
            if (f.is_open())
            {
                char    data[128];
                size_t  read_count = 0;
                int32_t new_offset = 0;
                f.seek(-15, ov::file::seek_end, new_offset);
                if (f.read(data, sizeof(data), read_count) && (read_count > 0))
                {
                    data[0]++;
                }
                else
                {
                    size_t write_count = 0;
                    f.write("This is a good test file!", 25u, write_count);
                }
            }

            ov::file f2 = static_cast<ov::file&&>(f);
            f2.close();

            ov::dir d = ov::fs::open_dir("/");
            if (d.is_open())
            {
                size_t         count = 0;
                ov::dir::entry entry;
                while (d.read(entry))
                {
                    count++;
                }
            }
        }

        while (true)
        {
            free_space++;
            total_size++;
            ov::this_thread::sleep_for(2000u);
        }
    }
};

static Test t;

/** @brief Entry point */
int main()
{
    // Configure clocks
    SystemClock_Config();
    PeriphCommonClock_Config();

    // Start operating system
    t.start();
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
