/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_SENSORS_CONSOLE_H
#define OV_SENSORS_CONSOLE_H

#include "i_barometric_altimeter.h"
#include "i_debug_console.h"

namespace ov
{

/** @brief Console command helpers for the sensors */
class sensors_console
{
  public:
    /** @brief Constructor */
    sensors_console(i_debug_console& console, i_barometric_altimeter& altimeter);

    /** @brief Register command handlers */
    void register_handlers();

  private:
    /** @brief Console */
    i_debug_console& m_console;
    /** @brief Altimeter */
    i_barometric_altimeter& m_altimeter;

    /** @brief Handler for the 'gnss' command */
    ov::i_debug_console::cmd_handler m_gnss_handler;
    /** @brief Handler for the 'alti' command */
    ov::i_debug_console::cmd_handler m_alti_handler;
    /** @brief Handler for the 'alticalib' command */
    ov::i_debug_console::cmd_handler m_alticalib_handler;
    /** @brief Handler for the 'accel' command */
    ov::i_debug_console::cmd_handler m_accel_handler;

    /** @brief Handler for the 'gnss' command */
    void gnss_handler(const char*);
    /** @brief Handler for the 'alti' command */
    void alti_handler(const char*);
    /** @brief Handler for the 'alticalib' command */
    void alticalib_handler(const char* new_alti);
    /** @brief Handler for the 'accel' command */
    void accel_handler(const char*);

    /** @brief Display gnss data */
    void display_gnss_data();
    /** @brief Display alti data */
    void display_alti_data();
    /** @brief Display accel data */
    void display_accel_data();
};

} // namespace ov

#endif // OV_SENSORS_CONSOLE_H
