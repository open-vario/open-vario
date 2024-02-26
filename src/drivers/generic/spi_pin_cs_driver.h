/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_SPI_PIN_CS_DRIVER_H
#define OV_SPI_PIN_CS_DRIVER_H

#include "i_output_pin.h"
#include "i_spi.h"

#include <initializer_list>

namespace ov
{

/** @brief SPI chip select driver implementation using generic output pin */
class spi_pin_cs_driver_base : public i_spi::i_cs_driver
{
  public:
    /** @brief Constructor */
    spi_pin_cs_driver_base(i_output_pin* cs_pins[], uint32_t cs_pins_count);

    /** @brief Initialize chip select lines states */
    bool init() override;

    /** @brief Enable the selected peripheral */
    void enable(const uint8_t peripheral) override;

    /** @brief Disable the selected peripheral */
    void disable(const uint8_t peripheral) override;

  private:
    /** @brief Pins to use as chip select lines */
    i_output_pin** m_cs_pins;
    /** @brief Number of pins */
    uint32_t m_cs_pins_count;
};

/** @brief SPI chip select driver implementation using generic output pin */
template <uint32_t CS_PINS_COUNT>
class spi_pin_cs_driver : public spi_pin_cs_driver_base
{
  public:
    /** @brief Constructor */
    spi_pin_cs_driver(std::initializer_list<i_output_pin*> cs_pins) : spi_pin_cs_driver_base(m_cs_pins, CS_PINS_COUNT)
    {
        size_t i = 0;
        for (auto& pin : cs_pins)
        {
            if (i < CS_PINS_COUNT)
            {
                m_cs_pins[i] = pin;
                i++;
            }
        }
    }

  private:
    /** @brief Pins to use as chip select lines */
    i_output_pin* m_cs_pins[CS_PINS_COUNT];
};

} // namespace ov

#endif // OV_SPI_PIN_CS_DRIVER_H
