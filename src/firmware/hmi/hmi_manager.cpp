
#include "hmi_manager.h"
#include "i_button.h"
#include "i_display.h"
#include "i_hmi_screen.h"
#include "os.h"

#include <YACSGL.h>
#include <YACSWL.h>

namespace ov
{

/** @brief Delay before triggering a 'long push' event */
static const uint32_t LONG_PUSH_DELAY = 1000u;

/** @brief Constructor */
hmi_manager::hmi_manager(
    i_display& display, i_debug_console& debug_console, i_button& previous_button, i_button& next_button, i_button& select_button)
    : m_display(display),
      m_buttons(),
      m_hmi_console(debug_console, *this),
      m_display_on(true),
      m_is_night_mode(true),
      m_current_screen(&m_splash_screen),
      m_next_screen(hmi_screen::splash),
      m_screens(),
      m_splash_screen(*this),
      m_thread()
{
    // Buttons
    m_buttons[0u].button = &previous_button;
    m_buttons[1u].button = &next_button;
    m_buttons[2u].button = &select_button;
    for (int i = 0u; i < static_cast<int>(button::count); i++)
    {
        m_buttons[i].is_pushed = false;
        m_buttons[i].pushed_ts = 0;
    }

    // Screens
    m_screens[0] = &m_splash_screen;
}

/** @brief Start the HMI manager */
bool hmi_manager::start()
{
    // Start thread
    auto thread_func = ov::thread_func::create<hmi_manager, &hmi_manager::thread_func>(*this);
    bool ret         = m_thread.start(thread_func, "HMI", 1u, nullptr);

    // Regoster console handlers
    m_hmi_console.register_handlers();

    return ret;
}

/** @brief Set the next screen */
void hmi_manager::set_next_screen(hmi_screen screen)
{
    m_next_screen = screen;
}

/** @brief Turn the display ON/OFF */
void hmi_manager::set_display(bool on)
{
    m_display_on = on;
}

/** @brief Turn the night mode ON/OFF */
void hmi_manager::set_night_mode(bool on)
{
    m_is_night_mode = on;
}

/** @brief HMI thread */
void hmi_manager::thread_func(void*)
{
    // Frame buffer
    uint8_t*       frame_buffer = m_display.get_frame_buffer();
    size_t         width        = m_display.get_width();
    size_t         height       = m_display.get_heigth();
    YACSGL_frame_t frame        = {static_cast<uint16_t>(width), static_cast<uint16_t>(height), 0, 0, frame_buffer};

    // Initialize screens
    for (int i = 0; i < static_cast<int>(hmi_screen::count); i++)
    {
        m_screens[i]->init(frame);
    }

    // Thread loop
    while (true)
    {
        // Handle buttons
        uint32_t current_ts = ov::os::now();
        for (int i = 0u; i < static_cast<int>(button::count); i++)
        {
            button_data& bt = m_buttons[i];

            // Get current state
            bool is_pushed = bt.button->is_pushed();
            if (is_pushed)
            {
                // Check if the button was released
                if (bt.is_pushed)
                {
                    // Trigger 'long push' event
                    if ((current_ts - bt.pushed_ts) >= LONG_PUSH_DELAY)
                    {
                        m_current_screen->event(static_cast<button>(i), button_event::long_push);
                    }
                }
                else
                {
                    // Trigger 'down' event
                    m_current_screen->event(static_cast<button>(i), button_event::down);

                    // Save timestamp
                    bt.pushed_ts = current_ts;
                }
            }
            else
            {
                // Check if the button was pushed
                if (bt.is_pushed)
                {
                    // Trigger 'short push' event
                    if ((current_ts - bt.pushed_ts) < LONG_PUSH_DELAY)
                    {
                        m_current_screen->event(static_cast<button>(i), button_event::short_push);
                    }

                    // Trigger 'up' event
                    m_current_screen->event(static_cast<button>(i), button_event::up);
                }
            }

            // Store new state
            bt.is_pushed = is_pushed;
        }

        // Update current screen
        if (m_current_screen->get_id() != m_next_screen)
        {
            // Switch screen
            m_current_screen = m_screens[static_cast<int>(m_next_screen)];
        }
        m_current_screen->set_night_mode(m_is_night_mode);
        m_current_screen->refresh(frame);

        // Refresh rate = 4FPS
        if (m_display.is_on() && !m_display_on)
        {
            m_display.turn_off();
        }
        if (!m_display.is_on() && m_display_on)
        {
            m_display.turn_on();
        }
        m_display.refresh();
        ov::this_thread::sleep_for(250u);
    }
}

} // namespace ov
