/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "dashboard3_screen.h"
#include "ov_data.h"

#include <cstdio>
#include <cstring>

namespace ov
{

/** @brief Constructor */
dashboard3_screen::dashboard3_screen(i_hmi_manager& hmi_manager) : base_screen(hmi_screen::dashboard3, hmi_manager) { }

/** @brief Button event */
void dashboard3_screen::event(button bt, button_event bt_event)
{
    if (bt_event == button_event::short_push)
    {
        if (bt == button::next)
        {
            switch_to_screen(hmi_screen::dashboard4);
        }
        if (bt == button::previous)
        {
            switch_to_screen(hmi_screen::dashboard2);
        }
    }
}

/** @brief Initialize the screen */
void dashboard3_screen::on_init(YACSGL_frame_t&)
{
    // Default values
    strcpy(m_lat_string, "LA: 00.00'00''N");
    strcpy(m_lon_string, "LO: 00.00'00''E");
    strcpy(m_track_angle_string, "TA: 000.0");

    // Latitude label
    YACSWL_label_init(&m_lat_label);
    YACSWL_label_set_text(&m_lat_label, m_lat_string);
    YACSWL_widget_set_border_width(&m_lat_label.widget, 0u);
    YACSWL_widget_set_pos(&m_lat_label.widget, 5u, 5u);

    // Longitude label
    YACSWL_label_init(&m_lon_label);
    YACSWL_label_set_text(&m_lon_label, m_lon_string);
    YACSWL_widget_set_border_width(&m_lon_label.widget, 0u);
    YACSWL_widget_set_pos(
        &m_lon_label.widget, 5u, YACSWL_widget_get_pos_y(&m_lat_label.widget) + YACSWL_widget_get_height(&m_lat_label.widget));

    // Track angle label
    YACSWL_label_init(&m_track_angle_label);
    YACSWL_label_set_text(&m_track_angle_label, m_track_angle_string);
    YACSWL_widget_set_border_width(&m_track_angle_label.widget, 0u);
    YACSWL_widget_set_pos(
        &m_track_angle_label.widget, 5u, YACSWL_widget_get_pos_y(&m_lon_label.widget) + YACSWL_widget_get_height(&m_lon_label.widget));

    // Add to root widget
    YACSWL_widget_add_child(&m_root_widget, &m_lat_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_lon_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_track_angle_label.widget);
}

/** @brief Refresh the contents of the screen */
void dashboard3_screen::on_refresh(YACSGL_frame_t&)
{
    // Update strings
    auto gnss = ov::data::get_gnss();
    if (gnss.is_valid)
    {
        uint32_t deg = 0;
        uint32_t min = 0;
        uint32_t sec = 0;
        char     ref = 0;

        // Latitude
        if (gnss.latitude >= 0)
        {
            ref = 'N';
        }
        else
        {
            ref = 'S';
            gnss.latitude *= -1.;
        }
        i_gnss::to_dms(gnss.latitude, deg, min, sec);
        snprintf(m_lat_string, sizeof(m_lat_string), "LA: %02ld.%02ld'%02ld''%c", deg, min, sec, ref);

        // Longitude
        if (gnss.longitude >= 0)
        {
            ref = 'E';
        }
        else
        {
            ref = 'W';
            gnss.longitude *= -1.;
        }
        i_gnss::to_dms(gnss.longitude, deg, min, sec);
        snprintf(m_lon_string, sizeof(m_lon_string), "LO: %02ld.%02ld'%02ld''%c", deg, min, sec, ref);

        // Track angle
        uint16_t angle = gnss.track_angle / 10u;
        uint16_t part  = gnss.track_angle - angle * 10u;
        snprintf(m_track_angle_string, sizeof(m_track_angle_string), "TA: %03d.%d", angle, part);
    }
    else
    {
        strcpy(m_lat_string, "LA: --.--'--''-");
        strcpy(m_lon_string, "LO: --.--'--''-");
        strcpy(m_track_angle_string, "TA: ---.-");
    }
}

} // namespace ov
