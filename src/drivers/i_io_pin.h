/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_IO_PIN_H
#define OV_I_IO_PIN_H

#include "i_input_pin.h"
#include "i_output_pin.h"

namespace ov
{

/** @brief Interface for pin drivers with both input and output capabilities implementations */
class i_io_pin : public i_input_pin, public i_output_pin
{
  public:
    /** @brief Destructor */
    virtual ~i_io_pin() { }
};

} // namespace ov

#endif // OV_I_IO_PIN_H
