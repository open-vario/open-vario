/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_SETTINGS_GLIDER_SCREEN_H
#define OV_SETTINGS_GLIDER_SCREEN_H

#include "base_screen.h"

namespace ov
{

/** @brief Display glider selection screen */
class settings_glider_screen : public base_screen
{
  public:
    /** @brief Constructor */
    settings_glider_screen(i_hmi_manager& hmi_manager);

    /** @brief Button event */
    void event(button bt, button_event bt_event) override;

  private:
    /** @brief Glider label */
    YACSWL_label_t m_glider_label;
    /** @brief Glider name label */
    YACSWL_label_t m_glider_name_label;
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

#endif // OV_SETTINGS_GLIDER_SCREEN_H
