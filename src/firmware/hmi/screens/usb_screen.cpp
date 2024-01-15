
#include "usb_screen.h"
#include "i_usb_cdc.h"

namespace ov
{

/** @brief Constructor */
usb_screen::usb_screen(i_hmi_manager& hmi_manager, i_usb_cdc& usb) : base_screen(hmi_screen::usb, hmi_manager), m_usb(usb) { }

/** @brief Button event */
void usb_screen::event(button bt, button_event bt_event)
{
    if (bt_event == button_event::short_push)
    {
        if (bt == button::next)
        {
            switch_to_screen(hmi_screen::settings);
        }
        if (bt == button::previous)
        {
            switch_to_screen(hmi_screen::ble);
        }
    }
}

/** @brief Initialize the screen */
void usb_screen::on_init(YACSGL_frame_t& frame)
{
    // USB label
    YACSWL_label_init(&m_usb_label);
    YACSWL_label_set_text(&m_usb_label, "USB status");
    YACSWL_widget_set_border_width(&m_usb_label.widget, 0u);
    YACSWL_widget_set_pos(&m_usb_label.widget, (frame.frame_x_width - YACSWL_widget_get_width(&m_usb_label.widget)) / 2u, 5u);

    // Connection status label
    YACSWL_label_init(&m_connection_status_label);
    YACSWL_label_set_text(&m_connection_status_label, m_not_connected_string);
    YACSWL_widget_set_border_width(&m_connection_status_label.widget, 0u);
    YACSWL_widget_set_pos(&m_connection_status_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_connection_status_label.widget)) / 2u,
                          YACSWL_widget_get_pos_y(&m_usb_label.widget) + YACSWL_widget_get_height(&m_usb_label.widget));

    // Add to root widget
    YACSWL_widget_add_child(&m_root_widget, &m_usb_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_connection_status_label.widget);
}

/** @brief Refresh the contents of the screen */
void usb_screen::on_refresh(YACSGL_frame_t& frame)
{
    // Update status
    if (m_usb.is_link_up())
    {
        YACSWL_label_set_text(&m_connection_status_label, m_connected_string);
    }
    else
    {
        YACSWL_label_set_text(&m_connection_status_label, m_not_connected_string);
    }
    YACSWL_widget_set_pos(&m_connection_status_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_connection_status_label.widget)) / 2u,
                          YACSWL_widget_get_pos_y(&m_usb_label.widget) + YACSWL_widget_get_height(&m_usb_label.widget));
}

} // namespace ov
