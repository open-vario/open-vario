/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_DEBUG_CONSOLE_H
#define OV_I_DEBUG_CONSOLE_H

#include "delegate.h"

#include <cstdint>

namespace ov
{

/** @brief Command handler function */
using handler_func = delegate<void, const char*>;
/** @brief Periodic command handler function */
using periodic_handler_func = delegate<void>;

/** @brief Interface for the debug console implementations */
class i_debug_console
{
  public:
    /** @brief Command handler */
    struct cmd_handler
    {
        /** @brief Command */
        const char* cmd;
        /** @brief Help message */
        const char* help;
        /** @brief Handler */
        handler_func handler;
        /** @brief Next handle */
        cmd_handler* next;
        /** @brief Indicate if at least 1 parameter is needed */
        bool params_needed;
    };

    /** @brief Destructor */
    virtual ~i_debug_console() { }

    /** @brief Register a command handler */
    virtual void register_handler(cmd_handler& handler) = 0;

    /** @brief Write a null-terminated string to the debug console */
    virtual void write(const char* str) = 0;

    /** @brief Write a null-terminated string as a line to the debug console */
    virtual void write_line(const char* str) = 0;

    /** \brief Get the next parameter in the command line */
    virtual const char* get_next_param(const char* current_param) = 0;

    /** @brief Start a periodic handler */
    virtual void start_periodic(periodic_handler_func func, uint32_t ms_period) = 0;
};

} // namespace ov

#endif // OV_I_DEBUG_CONSOLE_H
