/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_DASHBOARD3_SCREEN_H
#define OV_DASHBOARD3_SCREEN_H

#include "base_screen.h"

namespace ov
{

/** @brief Dashboard 3 screen */
class dashboard3_screen : public base_screen
{
  public:
    /** @brief Constructor */
    dashboard3_screen(i_hmi_manager& hmi_manager);

    /** @brief Button event */
    void event(button bt, button_event bt_event) override;

  private:
    /** @brief Latitude label */
    YACSWL_label_t m_lat_label;
    /** @brief Longitude label */
    YACSWL_label_t m_lon_label;
    /** @brief Track angle label */
    YACSWL_label_t m_track_angle_label;
    /** @brief Latitude string */
    char m_lat_string[18u];
    /** @brief Longitude string */
    char m_lon_string[18u];
    /** @brief Track angle string */
    char m_track_angle_string[12u];

    /** @brief Initialize the screen */
    void on_init(YACSGL_frame_t& frame) override;

    /** @brief Refresh the contents of the screen */
    void on_refresh(YACSGL_frame_t& frame) override;
};

} // namespace ov

#endif // OV_DASHBOARD3_SCREEN_H
