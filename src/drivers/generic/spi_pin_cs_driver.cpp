/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "spi_pin_cs_driver.h"

namespace ov
{
/** @brief Constructor */
spi_pin_cs_driver_base::spi_pin_cs_driver_base(i_output_pin* cs_pins[], uint32_t cs_pins_count)
    : m_cs_pins(cs_pins), m_cs_pins_count(cs_pins_count)
{
}

/** @brief Initialize chip select lines states */
bool spi_pin_cs_driver_base::init()
{
    for (uint32_t i = 0; i < m_cs_pins_count; i++)
    {
        m_cs_pins[i]->set_high();
    }

    return true;
}

/** @brief Enable the selected peripheral */
void spi_pin_cs_driver_base::enable(const uint8_t peripheral)
{
    if (peripheral < m_cs_pins_count)
    {
        m_cs_pins[peripheral]->set_low();
    }
}

/** @brief Disable the selected peripheral */
void spi_pin_cs_driver_base::disable(const uint8_t peripheral)
{
    if (peripheral < m_cs_pins_count)
    {
        m_cs_pins[peripheral]->set_high();
    }
}

} // namespace ov
