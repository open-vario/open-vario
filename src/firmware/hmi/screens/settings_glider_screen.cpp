/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "settings_glider_screen.h"
#include "i_hmi_manager.h"
#include "ov_config.h"

namespace ov
{

/** @brief Constructor */
settings_glider_screen::settings_glider_screen(i_hmi_manager& hmi_manager) : base_screen(hmi_screen::settings_glider, hmi_manager) { }

/** @brief Button event */
void settings_glider_screen::event(button bt, button_event bt_event)
{
    if (bt_event == button_event::short_push)
    {
        if (bt == button::next)
        {
            switch_to_screen(hmi_screen::settings_display);
        }
        if (bt == button::previous)
        {
            switch_to_screen(hmi_screen::settings_exit);
        }
        if (bt == button::select)
        {
            auto& config = ov::config::get();
            if (config.glider == 4u)
            {
                config.glider = 1u;
            }
            else
            {
                config.glider++;
            }
        }
    }
}

/** @brief Initialize the screen */
void settings_glider_screen::on_init(YACSGL_frame_t& frame)
{
    // Glider label
    YACSWL_label_init(&m_glider_label);
    YACSWL_label_set_text(&m_glider_label, "Select glider");
    YACSWL_widget_set_border_width(&m_glider_label.widget, 0u);
    YACSWL_widget_set_pos(&m_glider_label.widget, (frame.frame_x_width - YACSWL_widget_get_width(&m_glider_label.widget)) / 2u, 5u);

    // Glider name label
    YACSWL_label_init(&m_glider_name_label);
    YACSWL_label_set_text(&m_glider_name_label, "");
    YACSWL_widget_set_border_width(&m_glider_name_label.widget, 0u);
    YACSWL_widget_set_pos(&m_glider_name_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_glider_name_label.widget)) / 2u,
                          5u + YACSWL_widget_get_pos_y(&m_glider_label.widget) + YACSWL_widget_get_height(&m_glider_label.widget));

    // Add to root widget
    YACSWL_widget_add_child(&m_root_widget, &m_glider_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_glider_name_label.widget);
}

/** @brief Refresh the contents of the screen */
void settings_glider_screen::on_refresh(YACSGL_frame_t& frame)
{
    auto&       config      = ov::config::get();
    const char* glider_name = nullptr;
    switch (config.glider)
    {
        default:
            [[fallthrough]];
        case 1:
            glider_name = config.glider1_name;
            break;
        case 2:
            glider_name = config.glider2_name;
            break;
        case 3:
            glider_name = config.glider3_name;
            break;
        case 4:
            glider_name = config.glider4_name;
            break;
    }
    YACSWL_label_set_text(&m_glider_name_label, glider_name);
    YACSWL_widget_set_pos(&m_glider_name_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_glider_name_label.widget)) / 2u,
                          5u + YACSWL_widget_get_pos_y(&m_glider_label.widget) + YACSWL_widget_get_height(&m_glider_label.widget));
}

} // namespace ov
