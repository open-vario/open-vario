
#ifndef OV_FS_CONSOLE_H
#define OV_FS_CONSOLE_H

#include "i_debug_console.h"

namespace ov
{

/** @brief Console command helpers for the filesystem */
class fs_console
{
  public:
    /** @brief Constructor */
    fs_console(i_debug_console& console);

    /** @brief Register command handlers */
    void register_handlers();

  private:
    /** @brief Console */
    i_debug_console& m_console;

    /** @brief Handler for the 'ls' command */
    ov::i_debug_console::cmd_handler m_ls_handler;
    /** @brief Handler for the 'df' command */
    ov::i_debug_console::cmd_handler m_df_handler;
    /** @brief Handler for the 'cat' command */
    ov::i_debug_console::cmd_handler m_cat_handler;
    /** @brief Handler for the 'hexdump' command */
    ov::i_debug_console::cmd_handler m_hexdump_handler;

    /** @brief Handler for the 'ls' command */
    void ls_handler(const char* dir_path);
    /** @brief Handler for the 'df' command */
    void df_handler(const char*);
    /** @brief Handler for the 'cat' command */
    void cat_handler(const char* file_path);
    /** @brief Handler for the 'hexdump' command */
    void hexdump_handler(const char* file_path);
};

} // namespace ov

#endif // OV_FS_CONSOLE_H
