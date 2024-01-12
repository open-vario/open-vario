
#include "ble_screen.h"
#include "i_ble_stack.h"

namespace ov
{

/** @brief Constructor */
ble_screen::ble_screen(i_hmi_manager& hmi_manager, i_ble_stack& ble_stack)
    : base_screen(hmi_screen::ble, hmi_manager), m_ble_stack(ble_stack)
{
}

/** @brief Button event */
void ble_screen::event(button bt, button_event bt_event)
{
    if (bt_event == button_event::short_push)
    {
        if (bt == button::next)
        {
            switch_to_screen(hmi_screen::gnss);
        }
        if (bt == button::previous)
        {
            switch_to_screen(hmi_screen::flight);
        }
    }
}

/** @brief Initialize the screen */
void ble_screen::on_init(YACSGL_frame_t& frame)
{
    // BLE label
    YACSWL_label_init(&m_ble_label);
    YACSWL_label_set_text(&m_ble_label, "BLE status");
    YACSWL_widget_set_border_width(&m_ble_label.widget, 0u);
    YACSWL_widget_set_pos(&m_ble_label.widget, (frame.frame_x_width - YACSWL_widget_get_width(&m_ble_label.widget)) / 2u, 5u);

    // Connection status label
    YACSWL_label_init(&m_connection_status_label);
    YACSWL_label_set_text(&m_connection_status_label, m_not_connected_string);
    YACSWL_widget_set_border_width(&m_connection_status_label.widget, 0u);
    YACSWL_widget_set_pos(&m_connection_status_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_connection_status_label.widget)) / 2u,
                          YACSWL_widget_get_pos_y(&m_ble_label.widget) + YACSWL_widget_get_height(&m_ble_label.widget));

    // Add to root widget
    YACSWL_widget_add_child(&m_root_widget, &m_ble_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_connection_status_label.widget);
}

/** @brief Refresh the contents of the screen */
void ble_screen::on_refresh(YACSGL_frame_t& frame)
{
    // Update status
    if (m_ble_stack.is_device_connected())
    {
        YACSWL_label_set_text(&m_connection_status_label, m_connected_string);
    }
    else
    {
        YACSWL_label_set_text(&m_connection_status_label, m_not_connected_string);
    }
    YACSWL_widget_set_pos(&m_connection_status_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_connection_status_label.widget)) / 2u,
                          YACSWL_widget_get_pos_y(&m_ble_label.widget) + YACSWL_widget_get_height(&m_ble_label.widget));
}

} // namespace ov
