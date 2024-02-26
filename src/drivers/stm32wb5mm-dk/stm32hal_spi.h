/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_STM32HAL_SPI_H
#define OV_STM32HAL_SPI_H

#include "i_spi.h"
#include "semaphore.h"

#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_spi.h"

namespace ov
{

/** @brief SPI driver implementation using STM32HAL */
class stm32hal_spi : public i_spi
{
  public:
    /** @brief Constructor */
    stm32hal_spi(SPI_TypeDef* instance, uint32_t baudrate, polarity pol, phase pha, i_cs_driver& cs_driver);

    /** @brief Initialize the driver */
    bool init();

    /** @brief Transfer data through the SPI */
    bool xfer(const xfer_desc& xfer) override;

  private:
    /** @brief Chip select driver */
    i_cs_driver& m_cs_driver;
    /** @brief HAL SPI handle */
    SPI_HandleTypeDef m_spi;
    /** @brief Baudrate */
    uint32_t m_baudrate;
    /** @brief End of transfer semaphore */
    semaphore m_xfer_sem;

    /** @brief Compute the prescaler value */
    uint32_t compute_prescaler();

    /** @brief  Rx completed callback */
    static void rx_completed(SPI_HandleTypeDef* handle);
    /** @brief  Tx completed callback */
    static void tx_completed(SPI_HandleTypeDef* handle);
};

} // namespace ov

#endif // OV_STM32HAL_SPI_H
