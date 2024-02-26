/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_STM32HAL_I2C_H
#define OV_STM32HAL_I2C_H

#include "i_i2c.h"
#include "semaphore.h"

#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_i2c.h"

namespace ov
{

/** @brief I2C driver implementation using STM32HAL */
class stm32hal_i2c : public i_i2c
{
  public:
    /** @brief Constructor */
    stm32hal_i2c(I2C_TypeDef* instance);

    /** @brief Initialize the driver */
    bool init();

    /** @brief Transfer data through the I2C */
    bool xfer(const uint8_t slave_address, const xfer_desc& xfer) override;

    /** @brief IRQ handler */
    void irq_handler();

  private:
    /** @brief HAL I2C handle */
    I2C_HandleTypeDef m_i2c;

    /** @brief Current transfer */
    const xfer_desc* m_xfer;

    /** @brief Current error status */
    volatile error m_error;
};

} // namespace ov

#endif // OV_STM32HAL_I2C_H
