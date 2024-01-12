
#include "recorder_console.h"
#include "i_flight_recorder.h"

#include <cstdio>
#include <cstring>

namespace ov
{

/** @brief Constructor */
recorder_console::recorder_console(i_debug_console& console, i_flight_recorder& recorder)
    : m_console(console),
      m_recorder(recorder),
      m_flstatus_handler{"flstatus",
                         "Display flight status",
                         ov::handler_func::create<recorder_console, &recorder_console::flstatus_handler>(*this),
                         nullptr,
                         false}
{
}

/** @brief Register command handlers */
void recorder_console::register_handlers()
{
    m_console.register_handler(m_flstatus_handler);
}

/** @brief Handler for the 'flstatus' command */
void recorder_console::flstatus_handler(const char*)
{
    switch (m_recorder.get_status())
    {
        case i_flight_recorder::status::stopped:
        {
            m_console.write_line("Waiting for takeoff!");
        }
        break;

        case i_flight_recorder::status::starting:
        {
            m_console.write_line("Takeoff in progress!");
        }
        break;

        case i_flight_recorder::status::started:
        {
            // Convert flight duration in hh:mm:ss
            char     duration_string[64u];
            uint32_t duration   = m_recorder.get_recording_duration();
            uint32_t duration_h = duration / 3600u;
            uint32_t duration_m = (duration - (duration_h * 3600u)) / 60u;
            uint32_t duration_s = (duration - (duration_h * 3600u) - (duration_m * 60u));
            snprintf(duration_string, sizeof(duration_string), "%02ldh %02ldmin %02lds", duration_h, duration_m, duration_s);

            m_console.write("Flying since : ");
            m_console.write_line(duration_string);
        }
        break;

        case i_flight_recorder::status::stopping:
        {
            m_console.write_line("Landing in progress!");
        }
        break;

        default:
        {
            // Shall never happen ;)
            m_console.write_line("Oopsie!");
        }
        break;
    }
}

} // namespace ov
