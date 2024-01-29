
#include "settings_exit_screen.h"
#include "ov_config.h"

namespace ov
{

/** @brief Constructor */
settings_exit_screen::settings_exit_screen(i_hmi_manager& hmi_manager) : base_screen(hmi_screen::settings_exit, hmi_manager) { }

/** @brief Button event */
void settings_exit_screen::event(button bt, button_event bt_event)
{
    if (bt_event == button_event::short_push)
    {
        if (bt == button::next)
        {
            switch_to_screen(hmi_screen::settings_glider);
        }
        if (bt == button::previous)
        {
            switch_to_screen(hmi_screen::settings_display);
        }
        if (bt == button::select)
        {
            ov::config::save();
            switch_to_screen(hmi_screen::dashboard1);
        }
    }
}

/** @brief Initialize the screen */
void settings_exit_screen::on_init(YACSGL_frame_t& frame)
{
    // Exit label
    YACSWL_label_init(&m_exit_label);
    YACSWL_label_set_text(&m_exit_label, "Exit");
    YACSWL_widget_set_border_width(&m_exit_label.widget, 0u);
    YACSWL_widget_set_pos(&m_exit_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_exit_label.widget)) / 2u,
                          (frame.frame_y_heigth - YACSWL_widget_get_height(&m_exit_label.widget)) / 2u);

    // Add to root widget
    YACSWL_widget_add_child(&m_root_widget, &m_exit_label.widget);
}

} // namespace ov
