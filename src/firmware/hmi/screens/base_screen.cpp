
#include "base_screen.h"
#include "i_hmi_manager.h"

namespace ov
{

/** @brief Constructor */
base_screen::base_screen(hmi_screen screen_id, i_hmi_manager& hmi_manager)
    : m_root_widget(), m_screen_id(screen_id), m_hmi_manager(hmi_manager)
{
}

/** @brief Initialize the screen */
void base_screen::init(YACSGL_frame_t& frame)
{
    // Root widget
    YACSWL_widget_init(&m_root_widget);
    YACSWL_widget_set_size(&m_root_widget, frame.frame_x_width - 1u, frame.frame_y_heigth - 1u);
    YACSWL_widget_set_border_width(&m_root_widget, 0u);

    // Specific init
    on_init(frame);
}

/** @brief Refresh the contents of the screen */
void base_screen::refresh(YACSGL_frame_t& frame)
{
    // Specific refresh
    on_refresh(frame);

    // Draw widgets
    YACSWL_widget_draw(&m_root_widget, &frame);
}

/** @brief Set the night mode */
void base_screen::set_night_mode(bool is_on)
{
    if (is_on)
    {
        YACSWL_widget_set_foreground_color(&m_root_widget, YACSGL_P_WHITE);
        YACSWL_widget_set_background_color(&m_root_widget, YACSGL_P_BLACK);
    }
    else
    {
        YACSWL_widget_set_foreground_color(&m_root_widget, YACSGL_P_BLACK);
        YACSWL_widget_set_background_color(&m_root_widget, YACSGL_P_WHITE);
    }
}

/** @brief Switch to the specified screen */
void base_screen::switch_to_screen(hmi_screen screen)
{
    m_hmi_manager.set_next_screen(screen);
}

} // namespace ov
