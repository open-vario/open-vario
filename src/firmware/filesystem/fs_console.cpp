
#include "fs_console.h"
#include "fs.h"

#include <stdio.h>

namespace ov
{

/** @brief Constructor */
fs_console::fs_console(i_debug_console& console)
    : m_console(console),
      m_ls_handler{"ls",
                   "List the contents of the specified directory",
                   ov::handler_func::create<fs_console, &fs_console::ls_handler>(*this),
                   nullptr,
                   false},
      m_df_handler{
          "df", "Display the storage memory usage", ov::handler_func::create<fs_console, &fs_console::df_handler>(*this), nullptr, false},
      m_cat_handler{"cat",
                    "Display the contents of the specified file",
                    ov::handler_func::create<fs_console, &fs_console::cat_handler>(*this),
                    nullptr,
                    true},
      m_hexdump_handler{"hexdump",
                        "Display the contents of the specified file as hexadecimal values",
                        ov::handler_func::create<fs_console, &fs_console::hexdump_handler>(*this),
                        nullptr,
                        true}
{
}

/** @brief Register command handlers */
void fs_console::register_handlers()
{
    m_console.register_handler(m_ls_handler);
    m_console.register_handler(m_df_handler);
    m_console.register_handler(m_cat_handler);
    m_console.register_handler(m_hexdump_handler);
}

/** @brief Handler for the 'ls' command */
void fs_console::ls_handler(const char* dir_path)
{
    const char* dir = "/";
    if (dir_path)
    {
        dir = dir_path;
    }
    ov::dir d = ov::fs::open_dir(dir);
    if (d)
    {
        m_console.write_line("Type\tName\tSize");
        ov::dir::entry entry;
        while (d.read(entry))
        {
            if (entry.type == ov::dir::entry_type::dir)
            {
                m_console.write("D\t");
            }
            else
            {
                m_console.write("F\t");
            }
            m_console.write(entry.name);
            if (entry.type == ov::dir::entry_type::dir)
            {
                m_console.write_line("");
            }
            else
            {
                char buff[16u];
                snprintf(buff, sizeof(buff) - 1u, "\t%ld", entry.size);
                m_console.write_line(buff);
            }
        }
    }
    else
    {
        m_console.write("Unknown directory : ");
        m_console.write_line(dir);
    }
}

/** @brief Handler for the 'df' command */
void fs_console::df_handler(const char*)
{
    size_t free_space = 0;
    size_t total_size = 0;
    if (ov::fs::info(free_space, total_size))
    {
        char buff[16u];
        m_console.write("Storage memory usage : ");
        snprintf(buff, sizeof(buff) - 1u, "%d", (total_size - free_space));
        m_console.write(buff);
        m_console.write("/");
        snprintf(buff, sizeof(buff) - 1u, "%d", total_size);
        m_console.write(buff);
        m_console.write_line(" bytes");
    }
    else
    {
        m_console.write_line("Failed to get fileystem usage information");
    }
}

/** @brief Handler for the 'cat' command */
void fs_console::cat_handler(const char* file_path)
{
    ov::file f = ov::fs::open(file_path, ov::fs::o_rdonly);
    if (f)
    {
        char   buff[33u];
        size_t read_count = 0u;
        while (f.read(buff, sizeof(buff) - 1u, read_count) && (read_count != 0u))
        {
            buff[read_count] = 0;
            m_console.write(buff);
        }
        m_console.write_line("");
    }
    else
    {
        m_console.write("Unable to open file : ");
        m_console.write_line(file_path);
    }
}

/** @brief Handler for the 'hexdump' command */
void fs_console::hexdump_handler(const char* file_path)
{
    ov::file f = ov::fs::open(file_path, ov::fs::o_rdonly);
    if (f)
    {
        char   buff[32u];
        size_t read_count = 0u;
        while (f.read(buff, sizeof(buff), read_count) && (read_count != 0u))
        {
            size_t column = 0;
            for (size_t i = 0; i < read_count; i++)
            {
                char hex[5u];
                snprintf(hex, sizeof(hex), "0x%02x", static_cast<int>(buff[i]));
                m_console.write(hex);
                column++;
                if (column == 16u)
                {
                    m_console.write_line("");
                    column = 0;
                }
                else
                {
                    m_console.write(" ");
                }
            }
        }
        m_console.write_line("");
    }
    else
    {
        m_console.write("Unable to open file : ");
        m_console.write_line(file_path);
    }
}

} // namespace ov
