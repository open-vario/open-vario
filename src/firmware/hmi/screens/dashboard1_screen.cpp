
#include "dashboard1_screen.h"

#include <cstdio>
#include <cstring>

namespace ov
{

/** @brief Constructor */
dashboard1_screen::dashboard1_screen(i_hmi_manager& hmi_manager) : base_screen(hmi_screen::dashboard1, hmi_manager) { }

/** @brief Button event */
void dashboard1_screen::event(button bt, button_event bt_event)
{
    if (bt_event == button_event::short_push)
    {
        if (bt == button::next)
        {
            switch_to_screen(hmi_screen::dashboard2);
        }
        if (bt == button::previous)
        {
            switch_to_screen(hmi_screen::settings);
        }
    }
}

/** @brief Initialize the screen */
void dashboard1_screen::on_init(YACSGL_frame_t&)
{
    // Default values
    strcpy(m_qnh_string, "A : 0000m");
    strcpy(m_pressure_string, "P : 0000.00mbar");
    strcpy(m_temperature_string, "T : 00.0C");

    // QNH label
    YACSWL_label_init(&m_qnh_label);
    YACSWL_label_set_text(&m_qnh_label, m_qnh_string);
    YACSWL_widget_set_border_width(&m_qnh_label.widget, 0u);
    YACSWL_widget_set_pos(&m_qnh_label.widget, 5u, 5u);

    // Pressure label
    YACSWL_label_init(&m_pressure_label);
    YACSWL_label_set_text(&m_pressure_label, m_pressure_string);
    YACSWL_widget_set_border_width(&m_pressure_label.widget, 0u);
    YACSWL_widget_set_pos(
        &m_pressure_label.widget, 5u, YACSWL_widget_get_pos_y(&m_qnh_label.widget) + YACSWL_widget_get_height(&m_qnh_label.widget));

    // Temperature label
    YACSWL_label_init(&m_temperature_label);
    YACSWL_label_set_text(&m_temperature_label, m_temperature_string);
    YACSWL_widget_set_border_width(&m_temperature_label.widget, 0u);
    YACSWL_widget_set_pos(&m_temperature_label.widget,
                          5u,
                          YACSWL_widget_get_pos_y(&m_pressure_label.widget) + YACSWL_widget_get_height(&m_pressure_label.widget));

    // Add to root widget
    YACSWL_widget_add_child(&m_root_widget, &m_qnh_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_pressure_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_temperature_label.widget);
}

/** @brief Refresh the contents of the screen */
void dashboard1_screen::on_refresh(YACSGL_frame_t&)
{
    // Update strings
}

} // namespace ov