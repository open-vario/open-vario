/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_GNSS_SCREEN_H
#define OV_GNSS_SCREEN_H

#include "base_screen.h"

namespace ov
{

/** @brief GNSS screen */
class gnss_screen : public base_screen
{
  public:
    /** @brief Constructor */
    gnss_screen(i_hmi_manager& hmi_manager);

    /** @brief Button event */
    void event(button bt, button_event bt_event) override;

  private:
    /** @brief GNSS label */
    YACSWL_label_t m_gnss_label;
    /** @brief Signal status label */
    YACSWL_label_t m_signal_status_label;
    /** @brief Date time label */
    YACSWL_label_t m_date_time_label;
    /** @brief Signal status string */
    char m_signal_status_string[10u];
    /** @brief Date time string */
    char m_date_time_string[22u];

    /** @brief Error string */
    static constexpr const char* m_error_string = "Error";
    /** @brief No signal string */
    static constexpr const char* m_no_signal_string = "No signal";

    /** @brief Initialize the screen */
    void on_init(YACSGL_frame_t& frame) override;

    /** @brief Refresh the contents of the screen */
    void on_refresh(YACSGL_frame_t& frame) override;
};

} // namespace ov

#endif // OV_GNSS_SCREEN_H
