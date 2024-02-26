/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "pin_button.h"
#include "i_input_pin.h"

namespace ov
{

/** @brief Constructor */
pin_button::pin_button(i_input_pin& pin, bool pushed_state) : m_pin(pin), m_pushed_state(pushed_state) { }

/** @brief Indicate if the button is in pushed state */
bool pin_button::is_pushed()
{
    return (m_pin.get_level() == m_pushed_state);
}

} // namespace ov
