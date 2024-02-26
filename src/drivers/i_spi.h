/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_SPI_H
#define OV_I_SPI_H

#include <cstddef>
#include <cstdint>

namespace ov
{

/** @brief Interface for SPI drivers implementations */
class i_spi
{
  public:
    /** @brief Interface for SPI chip select drivers implementations */
    class i_cs_driver
    {
      public:
        /** @brief Initialize chip select lines states */
        virtual bool init() = 0;

        /** @brief Enable the selected peripheral */
        virtual void enable(const uint8_t peripheral) = 0;

        /** @brief Disable the selected peripheral */
        virtual void disable(const uint8_t peripheral) = 0;
    };

    /** @brief Polarity */
    enum class polarity
    {
        /** @brief Low (clock idle state is 0) */
        low = 0u,
        /** @brief High (clock idle state is 1) */
        high = 1u
    };

    /** @brief Phase */
    enum class phase
    {
        /** @brief Low (data captured on first clock edge) */
        first = 0u,
        /** @brief High (data captured on second clock edge) */
        second = 1u
    };

    /** @brief SPI transfer description */
    struct xfer_desc
    {
        /** @brief Constructor */
        xfer_desc() : write_data(nullptr), read_data(nullptr), size(0u), cs(0u), keep_cs_active(false), next(nullptr) { }

        /** @brief Buffer to write */
        const uint8_t* write_data;
        /** @brief Buffer for read data */
        uint8_t* read_data;
        /** @brief Buffer size in bytes */
        uint32_t size;
        /** @brief Chip select line */
        uint8_t cs;
        /** @brief Keep chip select line active between transfers */
        bool keep_cs_active;
        /** @brief Next transfer */
        const xfer_desc* next;
    };

    /** @brief Transfer data through the SPI */
    virtual bool xfer(const xfer_desc& xfer) = 0;
};

} // namespace ov

#endif // OV_I_SPI_H
