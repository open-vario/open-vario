
#include "settings_display_screen.h"
#include "i_hmi_manager.h"
#include "ov_config.h"

namespace ov
{

/** @brief Constructor */
settings_display_screen::settings_display_screen(i_hmi_manager& hmi_manager) : base_screen(hmi_screen::settings_display, hmi_manager) { }

/** @brief Button event */
void settings_display_screen::event(button bt, button_event bt_event)
{
    if (bt_event == button_event::short_push)
    {
        if (bt == button::next)
        {
            switch_to_screen(hmi_screen::settings_exit);
        }
        if (bt == button::previous)
        {
            switch_to_screen(hmi_screen::settings_glider);
        }
        if (bt == button::select)
        {
            ov::config::get().is_night_mode_on = !ov::config::get().is_night_mode_on;
        }
    }
}

/** @brief Initialize the screen */
void settings_display_screen::on_init(YACSGL_frame_t& frame)
{
    // Display label
    YACSWL_label_init(&m_display_label);
    YACSWL_label_set_text(&m_display_label, "Display");
    YACSWL_widget_set_border_width(&m_display_label.widget, 0u);
    YACSWL_widget_set_pos(&m_display_label.widget, (frame.frame_x_width - YACSWL_widget_get_width(&m_display_label.widget)) / 2u, 5u);

    // Night mode label
    YACSWL_label_init(&m_night_mode_label);
    YACSWL_label_set_text(&m_night_mode_label, m_night_mode_off_string);
    YACSWL_widget_set_border_width(&m_night_mode_label.widget, 0u);
    YACSWL_widget_set_pos(&m_night_mode_label.widget,
                          5u,
                          5u + YACSWL_widget_get_pos_y(&m_display_label.widget) + YACSWL_widget_get_height(&m_display_label.widget));

    // Add to root widget
    YACSWL_widget_add_child(&m_root_widget, &m_display_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_night_mode_label.widget);
}

/** @brief Refresh the contents of the screen */
void settings_display_screen::on_refresh(YACSGL_frame_t&)
{
    if (ov::config::get().is_night_mode_on)
    {
        YACSWL_label_set_text(&m_night_mode_label, m_night_mode_on_string);
    }
    else
    {
        YACSWL_label_set_text(&m_night_mode_label, m_night_mode_off_string);
    }
}

} // namespace ov
