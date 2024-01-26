
#ifndef OV_BOARD_H
#define OV_BOARD_H

#include "i_board.h"

// Drivers
#include "soft_i2c.h"
#include "spi_pin_cs_driver.h"
#include "stm32hal_i2c.h"
#include "stm32hal_iopin.h"
#include "stm32hal_lpuart.h"
#include "stm32hal_qspi.h"
#include "stm32hal_spi.h"
#include "stm32hal_usart.h"
#include "stm32hal_usb_cdc.h"

// Peripherals
#include "barometric_altimeter.h"
#include "fake_button.h"
#include "ism330dhcx.h"
#include "ms56xx_i2c.h"
#include "nmea_gnss.h"
#include "pin_button.h"
#include "s25flxxxs.h"
#include "ssd1315.h"

// BLE stack
#include "stm32wb5mm_ble_stack.h"

namespace ov
{

/** @brief OpenVario board implementation for STM32WB5MM-DK */
class ov_board : public i_board
{
  public:
    /** @brief Constructor */
    ov_board();

    /** @brief Initialize the clocks */
    bool clock_init();

    /** @brief Initialize the peripherals */
    bool init();

    /** @brief Reset the board */
    void reset() override;

    /** @brief Get the debug serial port */
    i_serial& get_debug_port() override { return m_dbg_usart_drv; }

    /** @brief Get the USB CDC port */
    i_usb_cdc& get_usb_cdc() override { return m_usb_cdc_drv; }

    /** @brief Get the storage memory */
    i_storage_memory& get_storage_memory() override { return m_qspi_nor_flash; }

    /** @brief Get the display */
    i_display& get_display() override { return m_oled_display; }

    /** @brief Get the 'Previous' button */
    i_button& get_previous_button() override { return m_previous_button; }

    /** @brief Get the 'Next' button */
    i_button& get_next_button() override { return m_next_button; }

    /** @brief Get the 'Select' button */
    i_button& get_select_button() override { return m_select_button; }

    /** @brief Get the BLE stack */
    i_ble_stack& get_ble_stack() override { return m_ble_stack; }

    /** @brief Get the GNSS  */
    i_gnss& get_gnss() override { return m_gnss; }

    /** @brief Get the barometric altimeter */
    i_barometric_altimeter& get_altimeter() override { return m_altimeter; }

    /** @brief Get the accelerometer */
    i_accelerometer_sensor& get_accelerometer() override { return m_accelerometer_sensor; }

  private:
    /** @brief Debug USART driver */
    stm32hal_usart m_dbg_usart_drv;

    /** @brief USB CDC driver */
    stm32hal_usb_cdc m_usb_cdc_drv;

    /** @brief QSPI driver */
    stm32hal_qspi m_qspi_drv;

    /** @brief Chip select pin for OLED driver */
    stm32hal_iopin m_oled_cs_pin;
    /** @brief Chip select driver for SPI1 */
    spi_pin_cs_driver<1u> m_spi1_cs_drv;
    /** @brief SPI1 driver */
    stm32hal_spi m_spi1_drv;

    /** @brief SCL pin for the software I2C driver */
    stm32hal_iopin m_soft_i2c_drv_scl;
    /** @brief SDA pin for the software I2C driver */
    stm32hal_iopin m_soft_i2c_drv_sda;
    /** @brief Software i2C driver */
    soft_i2c m_soft_i2c_drv;

    /** @brief I2C driver for internal sensors */
    stm32hal_i2c m_internal_i2c_drv;

    /** @brief Storage memory */
    s25flxxxs m_qspi_nor_flash;

    /** @brief Reset pin for the display */
    stm32hal_iopin m_oled_reset_pin;
    /** @brief Data/control pin for the display */
    stm32hal_iopin m_oled_data_pin;
    /** @brief OLED display */
    ssd1315 m_oled_display;

    /** @brief Pin for the 'Next' button */
    stm32hal_iopin m_next_button_pin;
    /** @brief Pin for the 'Select' button */
    stm32hal_iopin m_select_button_pin;
    /** @brief 'Previous' button */
    fake_button m_previous_button;
    /** @brief 'Next' button */
    pin_button m_next_button;
    /** @brief 'Select' button */
    pin_button m_select_button;

    /** @brief BLE stack */
    stm32wb5mm_ble_stack m_ble_stack;

    /** @brief GNSS serial port */
    stm32hal_lpuart m_gnss_lpuart_drv;
    /** @brief LPUART mux pin */
    stm32hal_iopin m_lpuart_mux_pin;
    /** @brief GNSS (UBlox SAM-M8Q)*/
    nmea_gnss m_gnss;

    /** @brief Barometric sensor */
    ms56xx_i2c m_barometric_sensor;
    /** @brief Barometric altimeter */
    barometric_altimeter m_altimeter;

    /** @brief Accelerometer sensor */
    ism330dhcx m_accelerometer_sensor;

    /** @brief Initialize the HAL */
    bool hal_init();
    /** @brief Initialize I/Os */
    bool io_init();

    /** @brief System Clock Configuration */
    bool system_clock_config();
    /** @brief Peripherals Common Clock Configuration */
    bool periph_common_clock_config();
    /** @brief Delay function for software I2C driver */
    void soft_i2c_delay();
};

} // namespace ov

#endif // OV_BOARD_H
