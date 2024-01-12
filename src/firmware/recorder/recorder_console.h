
#ifndef OV_RECORDER_CONSOLE_H
#define OV_RECORDER_CONSOLE_H

#include "i_debug_console.h"

namespace ov
{

// Forward declarations
class i_flight_recorder;

/** @brief Console command helpers for the flight recorder */
class recorder_console
{
  public:
    /** @brief Constructor */
    recorder_console(i_debug_console& console, i_flight_recorder& recorder);

    /** @brief Register command handlers */
    void register_handlers();

  private:
    /** @brief Console */
    i_debug_console& m_console;
    /** @brief Flight recorder */
    i_flight_recorder& m_recorder;

    /** @brief Handler for the 'flstatus' command */
    ov::i_debug_console::cmd_handler m_flstatus_handler;

    /** @brief Handler for the 'flstatus' command */
    void flstatus_handler(const char*);
};

} // namespace ov

#endif // OV_RECORDER_CONSOLE_H
