
#ifndef OV_STM32HAL_QSPI_H
#define OV_STM32HAL_QSPI_H

#include "i_qspi.h"
#include "semaphore.h"

#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_qspi.h"

namespace ov
{

/** @brief QSPI driver implementation using STM32HAL */
class stm32hal_qspi : public i_qspi
{
  public:
    /** @brief Constructor */
    stm32hal_qspi();

    /** @brief Initialize the driver */
    bool init();

    /** @brief Execute a QSPI read command */
    bool read(const command& cmd, void* buffer, size_t size) override;

    /** @brief Execute a QSPI write command */
    bool write(const command& cmd, const void* buffer, size_t size) override;

    /** @brief Poll a register state */
    bool poll(uint8_t cmd, uint8_t mask, uint8_t value, uint32_t ms_timeout) override;

  private:
    /** @brief HAL QSPI handle */
    QSPI_HandleTypeDef m_qspi;
    /** @brief HAL DMA handle */
    DMA_HandleTypeDef m_hdma_qspi;
    /** @brief Rx semaphore */
    semaphore m_rx_sem;
    /** @brief Tx semaphore */
    semaphore m_tx_sem;

    /** @brief  Rx completed callback */
    static void rx_completed(QSPI_HandleTypeDef* handle);
    /** @brief  Tx completed callback */
    static void tx_completed(QSPI_HandleTypeDef* handle);
};

} // namespace ov

#endif // OV_STM32HAL_QSPI_H
