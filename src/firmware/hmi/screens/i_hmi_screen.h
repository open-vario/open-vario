/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_HMI_SCREEN_H
#define OV_I_HMI_SCREEN_H

#include "hmi.h"

#include <YACSGL.h>

namespace ov
{

/** @brief Interface for the HMI screens implementations */
class i_hmi_screen
{
  public:
    /** @brief Destructor */
    virtual ~i_hmi_screen() { }

    /** @brief Screen identifier */
    virtual hmi_screen get_id() const = 0;

    /** @brief Initialize the screen */
    virtual void init(YACSGL_frame_t& frame) = 0;

    /** @brief Button event */
    virtual void event(button bt, button_event bt_event) = 0;

    /** @brief Refresh the contents of the screen */
    virtual void refresh(YACSGL_frame_t& frame) = 0;

    /** @brief Set the night mode */
    virtual void set_night_mode(bool is_on) = 0;
};

} // namespace ov

#endif // OV_I_HMI_SCREEN_H
