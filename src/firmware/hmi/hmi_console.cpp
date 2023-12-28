
#include "hmi_console.h"
#include "hmi_manager.h"
#include "ov_config.h"

#include <cstring>

namespace ov
{

/** @brief Constructor */
hmi_console::hmi_console(i_debug_console& console, i_hmi_manager& hmi_manager)
    : m_console(console),
      m_hmi_manager(hmi_manager),
      m_disp_handler{
          "disp", "Turn ON/OFF the display", ov::handler_func::create<hmi_console, &hmi_console::disp_handler>(*this), nullptr, true},
      m_night_handler{
          "night", "Turn ON/OFF the night mode", ov::handler_func::create<hmi_console, &hmi_console::night_handler>(*this), nullptr, true}
{
}

/** @brief Register command handlers */
void hmi_console::register_handlers()
{
    m_console.register_handler(m_disp_handler);
    m_console.register_handler(m_night_handler);
}

/** @brief Handler for the 'disp' command */
void hmi_console::disp_handler(const char* mode)
{
    bool on = false;
    m_console.write("Display ");
    if ((strcmp(mode, "on") == 0) || (strcmp(mode, "1") == 0))
    {
        on = true;
        m_console.write_line("ON");
    }
    else
    {
        m_console.write_line("OFF");
    }
    m_hmi_manager.set_display(on);
}

/** @brief Handler for the 'night' command */
void hmi_console::night_handler(const char* mode)
{
    bool on = false;
    m_console.write("Night mode ");
    if ((strcmp(mode, "on") == 0) || (strcmp(mode, "1") == 0))
    {
        on = true;
        m_console.write_line("ON");
    }
    else
    {
        m_console.write_line("OFF");
    }
    ov::config::get().is_night_mode_on = on;
}

} // namespace ov
