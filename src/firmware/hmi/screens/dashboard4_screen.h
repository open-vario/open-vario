/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_DASHBOARD4_SCREEN_H
#define OV_DASHBOARD4_SCREEN_H

#include "base_screen.h"

namespace ov
{

/** @brief Dashboard 4 screen */
class dashboard4_screen : public base_screen
{
  public:
    /** @brief Constructor */
    dashboard4_screen(i_hmi_manager& hmi_manager);

    /** @brief Button event */
    void event(button bt, button_event bt_event) override;

  private:
    /** @brief Acceleration label */
    YACSWL_label_t m_accel_label;
    /** @brief Acceleration string */
    char m_accel_string[18u];

    /** @brief Initialize the screen */
    void on_init(YACSGL_frame_t& frame) override;

    /** @brief Refresh the contents of the screen */
    void on_refresh(YACSGL_frame_t& frame) override;
};

} // namespace ov

#endif // OV_DASHBOARD4_SCREEN_H
