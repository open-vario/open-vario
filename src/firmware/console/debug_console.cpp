/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "debug_console.h"
#include "i_board.h"
#include "i_serial.h"
#include "os.h"

#include <cstring>

namespace ov
{

/** @brief Constructor */
debug_console::debug_console(i_board& board, i_serial& serial_port)
    : m_board(board),
      m_serial_port(serial_port),
      m_thread(),
      m_handlers(nullptr),
      m_help_handler{"help",
                     "Display the list of available commands",
                     handler_func::create<debug_console, &debug_console::help_handler>(*this),
                     nullptr,
                     false},
      m_reset_handler{
          "reset", "Reset the system", handler_func::create<debug_console, &debug_console::reset_handler>(*this), nullptr, false}
{
}

/** @brief Start the console */
bool debug_console::start()
{
    register_handler(m_help_handler);
    register_handler(m_reset_handler);
    return m_thread.start(thread_func::create<debug_console, &debug_console::thread_func>(*this), "Debug console", 1u, nullptr);
}

/** @brief Register a command handler */
void debug_console::register_handler(cmd_handler& handler)
{
    if (m_handlers == nullptr)
    {
        m_handlers = &handler;
    }
    else
    {
        cmd_handler* current_handler = m_handlers;
        while (current_handler->next != nullptr)
        {
            current_handler = current_handler->next;
        }
        current_handler->next = &handler;
        handler.next          = nullptr;
    }
}

/** @brief Write a null-terminated string to the debug console */
void debug_console::write(const char* str)
{
    m_serial_port.write(str);
}

/** @brief Write a null-terminated string as a line to the debug console */
void debug_console::write_line(const char* str)
{
    m_serial_port.write(str);
    m_serial_port.write("\r\n");
}

/** \brief Get the next parameter in the command line */
const char* debug_console::get_next_param(const char* current_param)
{
    const char* next_param = nullptr;

    // Check parameters
    if (current_param)
    {
        // Look for the end of the current param
        next_param = current_param;
        while ((*next_param) != 0)
        {
            next_param++;
        }

        // Jump to next char = start of next param
        next_param++;

        // Check if a parameter is present
        if ((*next_param) == 0)
        {
            // No parameter available
            next_param = nullptr;
        }
    }

    return next_param;
}

/** @brief Start a periodic handler */
void debug_console::start_periodic(periodic_handler_func func, uint32_t ms_period)
{
    write_line("Press Ctrl+C or ESC to stop...");

    // Loop
    bool end = false;
    while (!end)
    {
        // Check for escape command
        char read_char = 0;
        while (m_serial_port.read(&read_char, 1u, 0u))
        {
            if ((read_char == 0x03) || (read_char == 0x1b))
            {
                end = true;
            }
        }
        if (!end)
        {
            // Call handler
            func.invoke();

            // Wait next period
            ov::this_thread::sleep_for(ms_period);
        }
    }

    write_line("End of periodic display");
}

/** @brief Console thread */
void debug_console::thread_func(void*)
{
    // Command buffer
    static constexpr size_t CMD_BUFFER_SIZE                  = 64u;
    char                    cmd_buffer[CMD_BUFFER_SIZE + 1u] = {};

    // Current command size
    uint32_t current_cmd_size = 0;

    // Thread loop
    write_line("\r\nOpen Vario console ready!");
    write(">");
    while (true)
    {
        // Wait for data
        char read_char = 0;
        if (m_serial_port.read(&read_char, 1u, ov::os::infinite_timeout_value()))
        {
            bool send_echo = true;
            bool new_cmd   = false;

            // Handle received byte
            switch (read_char)
            {
                case '\b': // Backspace
                    [[fallthrough]];
                case 0x7f: // DEL
                {
                    // Remove previous byte
                    if (current_cmd_size == 0)
                    {
                        send_echo = false;
                    }
                    else
                    {
                        cmd_buffer[current_cmd_size] = 0;
                        current_cmd_size--;
                    }
                }
                break;

                case '\r': // Carriage return
                {
                    // Command is available
                    new_cmd = true;
                }
                break;

                default:
                {
                    // Add byte to command buffer
                    if (current_cmd_size == CMD_BUFFER_SIZE)
                    {
                        send_echo = false;
                    }
                    else
                    {
                        cmd_buffer[current_cmd_size] = read_char;
                        current_cmd_size++;
                    }
                }
                break;
            }

            // Echo received char
            if (send_echo)
            {
                m_serial_port.write(&read_char, 1u);
            }

            // Check end of command
            if (new_cmd)
            {
                // Split parameters
                cmd_buffer[current_cmd_size] = 0;
                for (uint32_t i = 0; i < current_cmd_size; i++)
                {
                    if (cmd_buffer[i] == ' ')
                    {
                        cmd_buffer[i] = 0;
                    }
                }

                // Handle command
                write("\n");
                if (strlen(cmd_buffer) != 0)
                {
                    cmd_handler* handler = get_handler(cmd_buffer);
                    if (handler)
                    {
                        // Check if parameters are needed
                        const char* params = get_next_param(cmd_buffer);
                        if (!handler->params_needed || (handler->params_needed && params))
                        {
                            // Call dedicated handler
                            handler->handler(static_cast<const char*&&>(params));
                        }
                        else
                        {
                            write_line("Missing parameter(s)");
                        }
                    }
                    else
                    {
                        write_line("Unknown command");
                    }
                }

                // Reset for next command
                current_cmd_size = 0;
                memset(cmd_buffer, 0, sizeof(cmd_buffer));
                write(">");
            }
        }
    }
}

/** @brief Get the handler corresponding to a command */
debug_console::cmd_handler* debug_console::get_handler(const char* cmd)
{
    cmd_handler* handler         = nullptr;
    cmd_handler* current_handler = m_handlers;
    while (current_handler != nullptr)
    {
        if (strcmp(cmd, current_handler->cmd) == 0)
        {
            handler = current_handler;
            break;
        }
        current_handler = current_handler->next;
    }
    return handler;
}

/** @brief Help command handler */
void debug_console::help_handler(const char*)
{
    write_line("Available commands : ");

    cmd_handler* current_handler = m_handlers;
    while (current_handler != nullptr)
    {
        write(" - ");
        write(current_handler->cmd);
        write(" : ");
        write_line(current_handler->help);
        current_handler = current_handler->next;
    }
}

/** @brief Reset command handler */
void debug_console::reset_handler(const char*)
{
    write_line("System will now reset!");
    m_board.reset();
}

} // namespace ov
