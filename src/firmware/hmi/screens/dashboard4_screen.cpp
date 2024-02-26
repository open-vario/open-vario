/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "dashboard4_screen.h"
#include "ov_data.h"

#include <cstdio>
#include <cstring>

namespace ov
{

/** @brief Constructor */
dashboard4_screen::dashboard4_screen(i_hmi_manager& hmi_manager) : base_screen(hmi_screen::dashboard3, hmi_manager) { }

/** @brief Button event */
void dashboard4_screen::event(button bt, button_event bt_event)
{
    if (bt_event == button_event::short_push)
    {
        if (bt == button::next)
        {
            switch_to_screen(hmi_screen::flight);
        }
        if (bt == button::previous)
        {
            switch_to_screen(hmi_screen::dashboard3);
        }
    }
}

/** @brief Initialize the screen */
void dashboard4_screen::on_init(YACSGL_frame_t&)
{
    // Default values
    strcpy(m_accel_string, "A : 0.00g");

    // Acceleration label
    YACSWL_label_init(&m_accel_label);
    YACSWL_label_set_text(&m_accel_label, m_accel_string);
    YACSWL_widget_set_border_width(&m_accel_label.widget, 0u);
    YACSWL_widget_set_pos(&m_accel_label.widget, 5u, 5u);

    // Add to root widget
    YACSWL_widget_add_child(&m_root_widget, &m_accel_label.widget);
}

/** @brief Refresh the contents of the screen */
void dashboard4_screen::on_refresh(YACSGL_frame_t&)
{
    // Update strings
    auto accelerometer = ov::data::get_accelerometer();
    if (accelerometer.is_valid)
    {
        // Acceleration angle
        uint16_t accel = accelerometer.total_accel / 1000u;
        uint16_t part  = (accelerometer.total_accel - accel * 1000u) / 10u;
        snprintf(m_accel_string, sizeof(m_accel_string), "A : %d.%02dg", accel, part);
    }
    else
    {
        strcpy(m_accel_string, "A : -.--g");
    }
}

} // namespace ov
