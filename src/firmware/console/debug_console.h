
#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

#include "i_debug_console.h"
#include "thread.h"

namespace ov
{

// Forward declarations
class i_serial;

/** @brief Debug console */
class debug_console : public i_debug_console
{
  public:
    /** @brief Constructor */
    debug_console(i_serial& serial_port);

    /** @brief Start the console */
    bool start();

    /** @brief Register a command handler */
    void register_handler(cmd_handler& handler) override;

    /** @brief Write a null-terminated string to the debug console */
    void write(const char* str) override;

    /** @brief Write a null-terminated string as a line to the debug console */
    void write_line(const char* str) override;

    /** \brief Get the next parameter in the command line */
    const char* get_next_param(char* const current_param) override;

  private:
    /** @brief Debug serial port */
    i_serial& m_serial_port;
    /** @brief Console thread */
    thread<2048u> m_thread;
    /** @brief Command handlers */
    cmd_handler* m_handlers;
    /** @brief Help command handler */
    cmd_handler m_help_handler;

    /** @brief Console thread */
    void thread_func(void*);
    /** @brief Get the handler corresponding to a command */
    cmd_handler* get_handler(const char* cmd);

    /** @brief Help command handler */
    void help_handler(const char*);
};

} // namespace ov

#endif // DEBUG_CONSOLE_H
