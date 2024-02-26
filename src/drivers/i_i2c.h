/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_I2C_H
#define OV_I_I2C_H

#include <cstddef>
#include <cstdint>

namespace ov
{

/** @brief Interface for I2C drivers implementations */
class i_i2c
{
  public:
    /** @brief I2C errors */
    enum class error
    {
        /** @brief No error */
        success,
        /** @brief Arbitration lost */
        arbitration_lost,
        /** @brief Bus error */
        bus_error,
        /** @brief Nack received */
        nack,
        /** @brief Other */
        other
    };

    /** @brief I2C transfer description */
    struct xfer_desc
    {
        /** @brief Constructor */
        xfer_desc() : data(nullptr), size(0u), read(true), stop_cond(true), next(nullptr) { }

        /** @brief Buffer */
        uint8_t* data;
        /** @brief Buffer size in bytes */
        uint8_t size;
        /** @brief Read data on I2C bus */
        bool read;
        /** @brief Trigger a stop condition at the end of the transfer */
        bool stop_cond;
        /** @brief Next transfer */
        const xfer_desc* next;
    };

    /** @brief Transfer data through the I2C */
    virtual bool xfer(const uint8_t slave_address, const xfer_desc& xfer) = 0;
};

} // namespace ov

#endif // OV_I_I2C_H
