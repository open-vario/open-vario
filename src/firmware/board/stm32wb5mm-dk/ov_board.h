
#ifndef OV_BOARD_H
#define OV_BOARD_H

#include "i_board.h"

#include "stm32hal_qspi.h"
#include "stm32hal_usart.h"

#include "s25flxxxs.h"

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

  private:
    /** @brief Debug USART driver */
    stm32hal_usart m_dbg_usart_drv;
    /** @brief QSPI driver */
    stm32hal_qspi m_qspi_drv;

    /** @brief Storage memory */
    s25flxxxs m_qspi_nor_flash;

    /** @brief Initialize the HAL */
    bool hal_init();
    /** @brief Initialize I/Os*/
    bool io_init();
};

} // namespace ov

#endif // OV_BOARD_H
