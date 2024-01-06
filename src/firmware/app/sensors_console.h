
#ifndef OV_SENSORS_CONSOLE_H
#define OV_SENSORS_CONSOLE_H

#include "i_debug_console.h"

namespace ov
{

/** @brief Console command helpers for the sensors */
class sensors_console
{
  public:
    /** @brief Constructor */
    sensors_console(i_debug_console& console);

    /** @brief Register command handlers */
    void register_handlers();

  private:
    /** @brief Console */
    i_debug_console& m_console;

    /** @brief Handler for the 'gnss' command */
    ov::i_debug_console::cmd_handler m_gnss_handler;

    /** @brief Handler for the 'gnss' command */
    void gnss_handler(const char*);

    /** @brief Display gnss data */
    void display_gnss_data();
};

} // namespace ov

#endif // OV_SENSORS_CONSOLE_H
