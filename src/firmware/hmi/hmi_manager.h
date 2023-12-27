
#ifndef OV_HMI_MANAGER_H
#define OV_HMI_MANAGER_H

#include "hmi_console.h"
#include "i_hmi_manager.h"
#include "thread.h"

#include "dashboard1_screen.h"
#include "dashboard2_screen.h"
#include "dashboard3_screen.h"
#include "settings_display_screen.h"
#include "settings_exit_screen.h"
#include "settings_screen.h"
#include "splash_screen.h"

namespace ov
{

// Forward declarations
class i_display;
class i_button;
class i_hmi_screen;

/** @brief HMI manager */
class hmi_manager : public i_hmi_manager
{
  public:
    /** @brief Constructor */
    hmi_manager(
        i_display& display, i_debug_console& debug_console, i_button& previous_button, i_button& next_button, i_button& select_button);

    /** @brief Start the HMI manager */
    bool start();

    /** @brief Set the next screen */
    void set_next_screen(hmi_screen screen) override;

    /** @brief Turn the display ON/OFF */
    void set_display(bool on) override;

    /** @brief Turn the night mode ON/OFF */
    void set_night_mode(bool on) override;

    /** @brief Indicate if the night mode is ON */
    bool is_night_mode_on() override;

  private:
    /** @brief Button data */
    struct button_data
    {
        /** @brief Button */
        i_button* button;
        /** @brief Timestamp of pushed event */
        uint32_t pushed_ts;
        /** @brief Indicate if the button is pushed */
        bool is_pushed;
    };

    /** @brief Display */
    i_display& m_display;
    /** @brief Buttons */
    button_data m_buttons[static_cast<unsigned int>(button::count)];
    /** @brief Console commands helper */
    hmi_console m_hmi_console;
    /** @brief Indicate if the display is ON */
    bool m_display_on;
    /** @brief Indicate if the night mode is ON */
    bool m_is_night_mode;

    /** @brief Current screen */
    i_hmi_screen* m_current_screen;
    /** @brief Next screen */
    hmi_screen m_next_screen;
    /** @brief Screens */
    i_hmi_screen* m_screens[static_cast<unsigned int>(hmi_screen::count)];

    /** @brief Splash screen */
    splash_screen m_splash_screen;
    /** @brief Dashboard 1 screen */
    dashboard1_screen m_dashboard1_screen;
    /** @brief Dashboard 2 screen */
    dashboard2_screen m_dashboard2_screen;
    /** @brief Dashboard 3 screen */
    dashboard3_screen m_dashboard3_screen;
    /** @brief Settings screen */
    settings_screen m_settings_screen;
    /** @brief Display settings screen */
    settings_display_screen m_settings_display_screen;
    /** @brief Exit settings screen */
    settings_exit_screen m_settings_exit_screen;

    /** @brief HMI thread */
    thread<2048u> m_thread;

    /** @brief HMI thread */
    void thread_func(void*);
};

} // namespace ov

#endif // OV_HMI_MANAGER_H
