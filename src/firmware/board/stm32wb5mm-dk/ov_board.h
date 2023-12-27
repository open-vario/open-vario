
#ifndef OV_BOARD_H
#define OV_BOARD_H

#include "i_board.h"

// Drivers
#include "spi_pin_cs_driver.h"
#include "stm32hal_iopin.h"
#include "stm32hal_qspi.h"
#include "stm32hal_spi.h"
#include "stm32hal_usart.h"

// Peripherals
#include "fake_button.h"
#include "pin_button.h"
#include "s25flxxxs.h"
#include "ssd1315.h"

namespace ov
{

/** @brief OpenVario board implementation for STM32WB5MM-DK */
class ov_board : public i_board
{
  public:
    /** @brief Constructor */
    ov_board();

    /** @brief Initialize the peripherals */
    bool init();

    /** @brief Get the debug serial port */
    i_serial& get_debug_port() override { return m_dbg_usart_drv; }

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

  private:
    /** @brief Debug USART driver */
    stm32hal_usart m_dbg_usart_drv;
    /** @brief QSPI driver */
    stm32hal_qspi m_qspi_drv;

    /** @brief Chip select pin for OLED driver */
    stm32hal_iopin m_oled_cs_pin;
    /** @brief Chip select driver for SPI1 */
    spi_pin_cs_driver<1u> m_spi1_cs_drv;
    /** @brief SPI1 driver */
    stm32hal_spi m_spi1_drv;

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

    /** @brief Initialize the HAL */
    bool hal_init();
    /** @brief Initialize I/Os*/
    bool io_init();
};

} // namespace ov

#endif // OV_BOARD_H
