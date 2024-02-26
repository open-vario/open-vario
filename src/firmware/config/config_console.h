/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_CONFIG_CONSOLE_H
#define OV_CONFIG_CONSOLE_H

#include "i_debug_console.h"

namespace ov
{

/** @brief Console command helpers for the configuration */
class config_console
{
  public:
    /** @brief Constructor */
    config_console(i_debug_console& console);

    /** @brief Register command handlers */
    void register_handlers();

  private:
    /** @brief Console */
    i_debug_console& m_console;

    /** @brief Handler for the 'confdump' command */
    ov::i_debug_console::cmd_handler m_confdump_handler;
    /** @brief Handler for the 'confwrite' command */
    ov::i_debug_console::cmd_handler m_confwrite_handler;
    /** @brief Handler for the 'confsave' command */
    ov::i_debug_console::cmd_handler m_confsave_handler;
    /** @brief Handler for the 'confreset' command */
    ov::i_debug_console::cmd_handler m_confreset_handler;

    /** @brief Handler for the 'confdump' command */
    void confdump_handler(const char*);
    /** @brief Handler for the 'confwrite' command */
    void confwrite_handler(const char* param_id);
    /** @brief Handler for the 'confsave' command */
    void confsave_handler(const char*);
    /** @brief Handler for the 'confreset' command */
    void confreset_handler(const char*);
};

} // namespace ov

#endif // OV_CONFIG_CONSOLE_H
