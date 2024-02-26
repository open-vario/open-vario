/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_DASHBOARD1_SCREEN_H
#define OV_DASHBOARD1_SCREEN_H

#include "base_screen.h"

namespace ov
{

/** @brief Dashboard 1 screen */
class dashboard1_screen : public base_screen
{
  public:
    /** @brief Constructor */
    dashboard1_screen(i_hmi_manager& hmi_manager);

    /** @brief Button event */
    void event(button bt, button_event bt_event) override;

  private:
    /** @brief QNH label */
    YACSWL_label_t m_qnh_label;
    /** @brief Pressure label */
    YACSWL_label_t m_pressure_label;
    /** @brief Temperature label */
    YACSWL_label_t m_temperature_label;
    /** @brief QNH string */
    char m_qnh_string[12u];
    /** @brief Pressure string */
    char m_pressure_string[18u];
    /** @brief Temperature string */
    char m_temperature_string[12u];

    /** @brief Initialize the screen */
    void on_init(YACSGL_frame_t& frame) override;

    /** @brief Refresh the contents of the screen */
    void on_refresh(YACSGL_frame_t& frame) override;
};

} // namespace ov

#endif // OV_DASHBOARD1_SCREEN_H
