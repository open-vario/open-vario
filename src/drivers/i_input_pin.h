/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_INPUT_PIN_H
#define OV_I_INPUT_PIN_H

#include "io_pin.h"

namespace ov
{

/** @brief Interface for input pins drivers implementations */
class i_input_pin
{
  public:
    /** @brief Indicate if the pin level is low */
    virtual bool is_low() = 0;

    /** @brief Indicate if the pin level is high */
    virtual bool is_high() = 0;

    /** @brief Get the pin level */
    virtual io::level get_level() = 0;
};

} // namespace ov

#endif // OV_I_INPUT_PIN_H
