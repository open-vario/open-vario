
#include "gnss_screen.h"
#include "ov_data.h"

#include <YACSGL_font_5x7.h>

#include <cstdio>
#include <cstring>

namespace ov
{

/** @brief Constructor */
gnss_screen::gnss_screen(i_hmi_manager& hmi_manager) : base_screen(hmi_screen::gnss, hmi_manager) { }

/** @brief Button event */
void gnss_screen::event(button bt, button_event bt_event)
{
    if (bt_event == button_event::short_push)
    {
        if (bt == button::next)
        {
            switch_to_screen(hmi_screen::ble);
        }
        if (bt == button::previous)
        {
            switch_to_screen(hmi_screen::flight);
        }
    }
}

/** @brief Initialize the screen */
void gnss_screen::on_init(YACSGL_frame_t& frame)
{
    // Default values
    strcpy(m_signal_status_string, "00 sat(s)");
    strcpy(m_date_time_string, "0000-00-00T00:00:00Z");

    // GNSS label
    YACSWL_label_init(&m_gnss_label);
    YACSWL_label_set_text(&m_gnss_label, "GNSS status");
    YACSWL_widget_set_border_width(&m_gnss_label.widget, 0u);
    YACSWL_widget_set_pos(&m_gnss_label.widget, (frame.frame_x_width - YACSWL_widget_get_width(&m_gnss_label.widget)) / 2u, 5u);

    // Signal status label
    YACSWL_label_init(&m_signal_status_label);
    YACSWL_label_set_text(&m_signal_status_label, m_no_signal_string);
    YACSWL_widget_set_border_width(&m_signal_status_label.widget, 0u);
    YACSWL_widget_set_pos(&m_signal_status_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_signal_status_label.widget)) / 2u,
                          YACSWL_widget_get_pos_y(&m_gnss_label.widget) + YACSWL_widget_get_height(&m_gnss_label.widget));

    // Date time label
    YACSWL_label_init(&m_date_time_label);
    YACSWL_label_set_text(&m_date_time_label, m_date_time_string);
    YACSWL_label_set_font(&m_date_time_label, &YACSGL_font_5x7);
    YACSWL_widget_set_border_width(&m_date_time_label.widget, 0u);
    YACSWL_widget_set_pos(&m_date_time_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_date_time_label.widget)) / 2u,
                          YACSWL_widget_get_pos_y(&m_signal_status_label.widget) + YACSWL_widget_get_height(&m_signal_status_label.widget));

    // Add to root widget
    YACSWL_widget_add_child(&m_root_widget, &m_gnss_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_signal_status_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_date_time_label.widget);
}

/** @brief Refresh the contents of the screen */
void gnss_screen::on_refresh(YACSGL_frame_t& frame)
{
    // Update status
    auto gnss = ov::data::get_gnss();
    if (gnss.is_valid)
    {
        if (gnss.satellite_count == 0)
        {
            YACSWL_label_set_text(&m_signal_status_label, m_no_signal_string);
        }
        else
        {
            snprintf(m_signal_status_string, sizeof(m_signal_status_string), "%02d sat(s)", static_cast<int>(gnss.satellite_count));
            YACSWL_label_set_text(&m_signal_status_label, m_signal_status_string);
        }

        snprintf(m_date_time_string,
                 sizeof(m_date_time_string),
                 "%04d-%02d-%02dT%02d:%02d:%02dZ",
                 static_cast<int>(gnss.date.year) + 2000,
                 static_cast<int>(gnss.date.month),
                 static_cast<int>(gnss.date.day),
                 static_cast<int>(gnss.date.hour),
                 static_cast<int>(gnss.date.minute),
                 static_cast<int>(gnss.date.second));
        YACSWL_widget_set_displayed(&m_date_time_label.widget, true);
    }
    else
    {
        YACSWL_label_set_text(&m_signal_status_label, m_error_string);
        YACSWL_widget_set_displayed(&m_date_time_label.widget, false);
    }

    // Center aligned label
    YACSWL_widget_set_pos(&m_signal_status_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_signal_status_label.widget)) / 2u,
                          YACSWL_widget_get_pos_y(&m_gnss_label.widget) + YACSWL_widget_get_height(&m_gnss_label.widget));
}

} // namespace ov
