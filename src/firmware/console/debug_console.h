
#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

#include "i_debug_console.h"
#include "thread.h"

namespace ov
{

// Forward declarations
class i_board;
class i_serial;

/** @brief Debug console */
class debug_console : public i_debug_console
{
  public:
    /** @brief Constructor */
    debug_console(i_board& board, i_serial& serial_port);

    /** @brief Start the console */
    bool start();

    /** @brief Register a command handler */
    void register_handler(cmd_handler& handler) override;

    /** @brief Write a null-terminated string to the debug console */
    void write(const char* str) override;

    /** @brief Write a null-terminated string as a line to the debug console */
    void write_line(const char* str) override;

    /** \brief Get the next parameter in the command line */
    const char* get_next_param(const char* current_param) override;

    /** @brief Start a periodic handler */
    void start_periodic(periodic_handler_func func, uint32_t ms_period) override;

  private:
    /** @brief Board */
    i_board& m_board;
    /** @brief Debug serial port */
    i_serial& m_serial_port;
    /** @brief Console thread */
    thread<2048u> m_thread;
    /** @brief Command handlers */
    cmd_handler* m_handlers;
    /** @brief Help command handler */
    cmd_handler m_help_handler;
    /** @brief Reset command handler */
    cmd_handler m_reset_handler;

    /** @brief Console thread */
    void thread_func(void*);
    /** @brief Get the handler corresponding to a command */
    cmd_handler* get_handler(const char* cmd);

    /** @brief Help command handler */
    void help_handler(const char*);
    /** @brief Reset command handler */
    void reset_handler(const char*);
};

} // namespace ov

#endif // DEBUG_CONSOLE_H
