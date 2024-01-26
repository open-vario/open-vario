
#include "flight_screen.h"
#include "i_flight_recorder.h"
#include "i_hmi_manager.h"

#include <cstdio>

namespace ov
{

/** @brief Constructor */
flight_screen::flight_screen(i_hmi_manager& hmi_manager, i_flight_recorder& recorder)
    : base_screen(hmi_screen::settings_display, hmi_manager), m_recorder(recorder)
{
}

/** @brief Button event */
void flight_screen::event(button bt, button_event bt_event)
{
    if (bt_event == button_event::short_push)
    {
        if (bt == button::next)
        {
            switch_to_screen(hmi_screen::gnss);
        }
        if (bt == button::previous)
        {
            switch_to_screen(hmi_screen::dashboard4);
        }
        if (bt == button::select)
        {
            // Start/stop recording
            auto recorder_status = m_recorder.get_status();
            if (recorder_status <= i_flight_recorder::status::stopped)
            {
                m_recorder.start();
            }
            if ((recorder_status == i_flight_recorder::status::started) || (recorder_status == i_flight_recorder::status::started_error))
            {
                m_recorder.stop();
            }
        }
    }
}

/** @brief Initialize the screen */
void flight_screen::on_init(YACSGL_frame_t& frame)
{
    // Flight label
    YACSWL_label_init(&m_flight_label);
    YACSWL_label_set_text(&m_flight_label, "Flight");
    YACSWL_widget_set_border_width(&m_flight_label.widget, 0u);
    YACSWL_widget_set_pos(&m_flight_label.widget, (frame.frame_x_width - YACSWL_widget_get_width(&m_flight_label.widget)) / 2u, 5u);

    // Flight status label
    YACSWL_label_init(&m_flight_status_label);
    YACSWL_label_set_text(&m_flight_status_label, m_flight_start_string1);
    YACSWL_widget_set_border_width(&m_flight_status_label.widget, 0u);
    YACSWL_widget_set_pos(&m_flight_status_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_flight_status_label.widget)) / 2u,
                          4u + YACSWL_widget_get_pos_y(&m_flight_label.widget) + YACSWL_widget_get_height(&m_flight_label.widget));
    YACSWL_widget_set_margins(&m_flight_status_label.widget, 1u, 1u, 1u, 1u);

    // Flight start label
    YACSWL_label_init(&m_flight_start_label);
    YACSWL_label_set_text(&m_flight_start_label, m_flight_start_string2);
    YACSWL_widget_set_border_width(&m_flight_start_label.widget, 0u);
    YACSWL_widget_set_pos(&m_flight_start_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_flight_start_label.widget)) / 2u,
                          YACSWL_widget_get_pos_y(&m_flight_status_label.widget) + YACSWL_widget_get_height(&m_flight_status_label.widget));
    YACSWL_widget_set_margins(&m_flight_start_label.widget, 0u, 0u, 0u, 0u);

    // Add to root widget
    YACSWL_widget_add_child(&m_root_widget, &m_flight_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_flight_status_label.widget);
    YACSWL_widget_add_child(&m_root_widget, &m_flight_start_label.widget);
}

/** @brief Refresh the contents of the screen */
void flight_screen::on_refresh(YACSGL_frame_t& frame)
{
    // Get recorder status
    switch (m_recorder.get_status())
    {
        case i_flight_recorder::status::stopped:
        {
            YACSWL_label_set_text(&m_flight_status_label, m_flight_start_string1);
            YACSWL_label_set_text(&m_flight_start_label, m_flight_start_string2);
        }
        break;

        case i_flight_recorder::status::starting:
        {
            YACSWL_label_set_text(&m_flight_status_label, "Takeoff");
            YACSWL_label_set_text(&m_flight_start_label, "in progress!");
        }
        break;

        case i_flight_recorder::status::started:
        {
            // Convert flight duration in hh:mm:ss
            uint32_t duration   = m_recorder.get_recording_duration();
            uint32_t duration_h = duration / 3600u;
            uint32_t duration_m = (duration - (duration_h * 3600u)) / 60u;
            uint32_t duration_s = (duration - (duration_h * 3600u) - (duration_m * 60u));
            snprintf(m_flight_status_string, sizeof(m_flight_status_string), "%02ldh %02ldmin %02lds", duration_h, duration_m, duration_s);

            YACSWL_label_set_text(&m_flight_status_label, m_flight_status_string);
            YACSWL_label_set_text(&m_flight_start_label, "flying!");
        }
        break;

        case i_flight_recorder::status::started_error:
            [[fallthrough]];
        case i_flight_recorder::status::stopped_error:
        {
            YACSWL_label_set_text(&m_flight_status_label, "Recording");
            YACSWL_label_set_text(&m_flight_start_label, "error :(");
        }
        break;

        case i_flight_recorder::status::stopping:
        {
            YACSWL_label_set_text(&m_flight_status_label, "Landing");
            YACSWL_label_set_text(&m_flight_start_label, "in progress!");
        }
        break;

        default:
            // Shall never happen ;)
            break;
    }

    // Update widgets position
    YACSWL_widget_set_pos(&m_flight_status_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_flight_status_label.widget)) / 2u,
                          4u + YACSWL_widget_get_pos_y(&m_flight_label.widget) + YACSWL_widget_get_height(&m_flight_label.widget));
    YACSWL_widget_set_pos(&m_flight_start_label.widget,
                          (frame.frame_x_width - YACSWL_widget_get_width(&m_flight_start_label.widget)) / 2u,
                          YACSWL_widget_get_pos_y(&m_flight_status_label.widget) + YACSWL_widget_get_height(&m_flight_status_label.widget));
}

} // namespace ov
