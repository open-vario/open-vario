/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_SETTINGS_DISPLAY_SCREEN_H
#define OV_SETTINGS_DISPLAY_SCREEN_H

#include "base_screen.h"

namespace ov
{

/** @brief Display settings screen */
class settings_display_screen : public base_screen
{
  public:
    /** @brief Constructor */
    settings_display_screen(i_hmi_manager& hmi_manager);

    /** @brief Button event */
    void event(button bt, button_event bt_event) override;

  private:
    /** @brief Display label */
    YACSWL_label_t m_display_label;
    /** @brief Night mode label */
    YACSWL_label_t m_night_mode_label;
    /** @brief Night mode ON string */
    static constexpr const char* m_night_mode_on_string = "Night mode:ON";
    /** @brief Night mode OFF string */
    static constexpr const char* m_night_mode_off_string = "Night mode:OFF";

    /** @brief Initialize the screen */
    void on_init(YACSGL_frame_t& frame) override;

    /** @brief Refresh the contents of the screen */
    void on_refresh(YACSGL_frame_t& frame) override;
};

} // namespace ov

#endif // OV_SETTINGS_DISPLAY_SCREEN_H
