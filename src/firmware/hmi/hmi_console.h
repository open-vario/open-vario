
#ifndef OV_HMI_CONSOLE_H
#define OV_HMI_CONSOLE_H

#include "i_debug_console.h"

namespace ov
{

// Forward declarations
class i_hmi_manager;

/** @brief Console command helpers for the HMI */
class hmi_console
{
  public:
    /** @brief Constructor */
    hmi_console(i_debug_console& console, i_hmi_manager& hmi_manager);

    /** @brief Register command handlers */
    void register_handlers();

  private:
    /** @brief Console */
    i_debug_console& m_console;
    /** @brief HMI manager */
    i_hmi_manager& m_hmi_manager;

    /** @brief Handler for the 'disp' command */
    ov::i_debug_console::cmd_handler m_disp_handler;
    /** @brief Handler for the 'night' command */
    ov::i_debug_console::cmd_handler m_night_handler;

    /** @brief Handler for the 'disp' command */
    void disp_handler(const char* mode);
    /** @brief Handler for the 'night' command */
    void night_handler(const char* mode);
};

} // namespace ov

#endif // OV_HMI_CONSOLE_H
