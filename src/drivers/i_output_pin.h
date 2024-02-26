/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_OUTPUT_PIN_H
#define OV_I_OUTPUT_PIN_H

#include "io_pin.h"

namespace ov
{

/** @brief Interface for output pins drivers implementations */
class i_output_pin
{
  public:
    /** @brief Set the pin to low level */
    virtual void set_low() = 0;

    /** @brief Set the pin to high level */
    virtual void set_high() = 0;

    /** @brief Set the pin to a specified level */
    virtual void set_level(io::level level) = 0;
};

} // namespace ov

#endif // OV_I_OUTPUT_PIN_H
