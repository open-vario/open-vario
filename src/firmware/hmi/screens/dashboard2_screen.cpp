
#include "dashboard2_screen.h"
#include "ov_data.h"

#include <cstdio>
#include <cstring>

namespace ov
{

/** @brief Constructor */
dashboard2_screen::dashboard2_screen(i_hmi_manager& hmi_manager) : base_screen(hmi_screen::dashboard2, hmi_manager) { }

/** @brief Button event */
void dashboard2_screen::event(button bt, button_event bt_event)
{
    if (bt_event == button_event::short_push)
    {
        if (bt == button::next)
        {
            switch_to_screen(hmi_screen::dashboard3);
        }
        if (bt == button::previous)
        {
            switch_to_screen(hmi_screen::dashboard1);
        }
    }
}

/** @brief Initialize the screen */
void dashboard2_screen::on_init(YACSGL_frame_t&)
{
    // Default values
    strcpy(m_sink_rate_string, "SR: 00.0m/s");
    strcpy(m_glide_ratio_string, "GR: 00.0");
    strcpy(m_speed_string, "SP: 000.0km/h");

    // Sink rate label
    YACSWL_label_init(&m_sink_rate_label);
    YACSWL_label_set_text(&m_sink_rate_label, m_sink_rate_string);
    YACSWL_widget_set_border_width(&m_sink_rate_label.widget, 0u);
    YACSWL_widget_set_pos(&m_sink_rate_label.widget, 5u, 5u);

    // Glide ratio label
    YACSWL_label_init(&m_glide_ratio_label);
    YACSWL_label_set_text(&m_glide_ratio_label, m_glide_ratio_string);
    YACSWL_widget_set_border_width(&m_glide_ratio_label.widget, 0u);
    YACSWL_widget_set_pos(&m_glide_ratio_label.widget,
                          5u,
                          YACSWL_widget_get_pos_y(&m_sink_rate_label.widget) + YACSWL_widget_get_height(&m_sink_rate_label.widget));

    // Speed label
    YACSWL_label_init(&m_speed_label);
    YACSWL_label_set_text(&m_speed_label, m_speed_string);
    YACSWL_widget_set_border_width(&m_speed_label.widget, 0u);
    YACSWL_widget_set_pos(&m_speed_label.widget,
                          5u,
                          YACSWL_widget_get_pos_y(&m_glide_ratio_label.widget) + YACSWL_widget_get_height(&m_glide_ratio_label.widget));

    // Add to root widget
    YACSWL_widget_add_child(&m_root_widget, &m_sink_rate_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_glide_ratio_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_speed_label.widget);
}

/** @brief Refresh the contents of the screen */
void dashboard2_screen::on_refresh(YACSGL_frame_t&)
{
    // Update strings
    auto gnss = ov::data::get_gnss();
    if (gnss.is_valid)
    {
        // Speed => Use dam/h unit for computation to avoid precision loss
        uint32_t speed_damh = gnss.speed * 36u;
        uint32_t speed      = speed_damh / 100u;
        uint32_t part       = (speed_damh - speed * 100u) / 10u;
        snprintf(m_speed_string, sizeof(m_speed_string), "SP: %03ld.%ldkm/h", speed, part);
    }
    else
    {
        strcpy(m_speed_string, "SP: ---.-km/h");
    }
}

} // namespace ov
