
#include "splash_screen.h"

#include <YACSGL_font_5x7.h>

#include <cstdio>

namespace ov
{

/** @brief Constructor */
splash_screen::splash_screen(i_hmi_manager& hmi_manager)
    : base_screen(hmi_screen::splash, hmi_manager), m_name_label(), m_version_label(), m_progress_bar(), m_version_string()
{
}

/** @brief Initialize the screen */
void splash_screen::on_init(YACSGL_frame_t& frame)
{
    // Name label
    YACSWL_label_init(&m_name_label);
    YACSWL_label_set_text(&m_name_label, "Open Vario");
    YACSWL_widget_set_border_width(&m_name_label.widget, 0u);
    YACSWL_widget_set_pos(&m_name_label.widget, (frame.frame_x_width - YACSWL_widget_get_width(&m_name_label.widget)) / 2u, 5u);

    // Version label
    m_version_string[sizeof(m_version_string) - 1u] = 0;
    snprintf(m_version_string, sizeof(m_version_string) - 1u, "v%s.%s.%s", OPENVARIO_MAJOR, OPENVARIO_MINOR, OPENVARIO_FIX);
    YACSWL_label_init(&m_version_label);
    YACSWL_label_set_font(&m_version_label, &YACSGL_font_5x7);
    YACSWL_label_set_text(&m_version_label, m_version_string);
    YACSWL_widget_set_border_width(&m_version_label.widget, 0u);
    YACSWL_widget_set_pos(&m_version_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_version_label.widget)) / 2u,
                          YACSWL_widget_get_pos_y(&m_name_label.widget) + YACSWL_widget_get_height(&m_name_label.widget));

    // Progress bar
    YACSWL_progress_bar_init(&m_progress_bar);
    YACSWL_widget_set_pos(&m_progress_bar.widget, 5u, frame.frame_y_heigth - 20u);
    YACSWL_widget_set_size(&m_progress_bar.widget, frame.frame_x_width - 10u, 15u);

    // Add to root widget
    YACSWL_widget_add_child(&m_root_widget, &m_name_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_version_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_progress_bar.widget);
}

/** @brief Refresh the contents of the screen */
void splash_screen::on_refresh(YACSGL_frame_t& frame)
{
    // Fake progress ;)
    static uint8_t progress = 0;

    YACSWL_progress_bar_set_progress(&m_progress_bar, progress);

    progress += 20u;
    if (progress > 100u)
    {
        this->switch_to_screen(hmi_screen::dashboard1);
        progress = 0;
    }
}

} // namespace ov
