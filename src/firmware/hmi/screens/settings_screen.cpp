/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "settings_screen.h"

namespace ov
{

/** @brief Constructor */
settings_screen::settings_screen(i_hmi_manager& hmi_manager) : base_screen(hmi_screen::settings, hmi_manager) { }

/** @brief Button event */
void settings_screen::event(button bt, button_event bt_event)
{
    if (bt_event == button_event::short_push)
    {
        if (bt == button::next)
        {
            switch_to_screen(hmi_screen::dashboard1);
        }
        if (bt == button::previous)
        {
            switch_to_screen(hmi_screen::ble);
        }
        if (bt == button::select)
        {
            switch_to_screen(hmi_screen::settings_glider);
        }
    }
}

/** @brief Initialize the screen */
void settings_screen::on_init(YACSGL_frame_t& frame)
{
    // Settings label
    YACSWL_label_init(&m_settings_label);
    YACSWL_label_set_text(&m_settings_label, "Settings");
    YACSWL_widget_set_border_width(&m_settings_label.widget, 0u);
    YACSWL_widget_set_pos(&m_settings_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_settings_label.widget)) / 2u,
                          (frame.frame_y_heigth - YACSWL_widget_get_height(&m_settings_label.widget)) / 2u);

    // Add to root widget
    YACSWL_widget_add_child(&m_root_widget, &m_settings_label.widget);
}

} // namespace ov
