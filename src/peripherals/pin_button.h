/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_PIN_BUTTON_H
#define OV_PIN_BUTTON_H

#include "i_button.h"

namespace ov
{

// Forward declarations
class i_input_pin;

/** @brief Button implementation from an input pin */
class pin_button : public i_button
{
  public:
    /** @brief Constructor */
    pin_button(i_input_pin& pin, bool pushed_state);

    /** @brief Indicate if the button is in pushed state */
    bool is_pushed() override;

  private:
    /** @brief Corresponding input pin */
    i_input_pin& m_pin;
    /** @brief Pushed state */
    bool m_pushed_state;
};

} // namespace ov

#endif // OV_PIN_BUTTON_H
