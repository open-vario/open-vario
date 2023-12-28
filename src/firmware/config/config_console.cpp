
#include "config_console.h"
#include "fs.h"
#include "ov_config.h"

#include <stdio.h>
#include <stdlib.h>

namespace ov
{

/** @brief Constructor */
config_console::config_console(i_debug_console& console)
    : m_console(console),
      m_confdump_handler{"confdump",
                         "Display the firmware configuration values",
                         ov::handler_func::create<config_console, &config_console::confdump_handler>(*this),
                         nullptr,
                         false},
      m_confwrite_handler{"confwrite",
                          "Modify a firmware configuration value (id value)",
                          ov::handler_func::create<config_console, &config_console::confwrite_handler>(*this),
                          nullptr,
                          true},
      m_confsave_handler{"confsave",
                         "Save the current firmware configuration values",
                         ov::handler_func::create<config_console, &config_console::confsave_handler>(*this),
                         nullptr,
                         false},
      m_confreset_handler{"confreset",
                          "Reset the firmware configuration values to their default values",
                          ov::handler_func::create<config_console, &config_console::confreset_handler>(*this),
                          nullptr,
                          false}
{
}

/** @brief Register command handlers */
void config_console::register_handlers()
{
    m_console.register_handler(m_confdump_handler);
    m_console.register_handler(m_confwrite_handler);
    m_console.register_handler(m_confsave_handler);
    m_console.register_handler(m_confreset_handler);
}

/** @brief Handler for the 'confdump' command */
void config_console::confdump_handler(const char*)
{
    char                tmp[32u];
    static const char*  entry_types[] = {"int", "uint", "bool", "float", "double", "string"};
    const config_entry* entry         = ov::config::get_desc();
    int                 id            = 0;

    m_console.write_line("Id\tName\tType\tSize\tValue\tDefault");
    m_console.write_line("---------------------------------------------");
    while (entry->name != nullptr)
    {
        snprintf(tmp, sizeof(tmp), "%d\t", id);
        m_console.write(tmp);
        m_console.write(entry->name);
        m_console.write("\t");
        m_console.write(entry_types[static_cast<int>(entry->type)]);
        m_console.write("\t");
        snprintf(tmp, sizeof(tmp), "%d\t", static_cast<int>(entry->size));
        m_console.write(tmp);
        switch (entry->type)
        {
            case entry_type::sint:
            {
                int32_t*       val         = reinterpret_cast<int32_t*>(entry->value);
                const int32_t* default_val = reinterpret_cast<const int32_t*>(entry->default_value);
                snprintf(tmp, sizeof(tmp), "%ld\t", (*val));
                m_console.write(tmp);
                snprintf(tmp, sizeof(tmp), "%ld", (*default_val));
                m_console.write(tmp);
            }
            break;

            case entry_type::uint:
            {
                uint32_t*       val         = reinterpret_cast<uint32_t*>(entry->value);
                const uint32_t* default_val = reinterpret_cast<const uint32_t*>(entry->default_value);
                snprintf(tmp, sizeof(tmp), "%lu\t", (*val));
                m_console.write(tmp);
                snprintf(tmp, sizeof(tmp), "%lu", (*default_val));
                m_console.write(tmp);
            }
            break;

            case entry_type::boolean:
            {
                bool*       val         = reinterpret_cast<bool*>(entry->value);
                const bool* default_val = reinterpret_cast<const bool*>(entry->default_value);
                if (*val)
                {
                    m_console.write("true\t");
                }
                else
                {
                    m_console.write("false\t");
                }
                if (*default_val)
                {
                    m_console.write("true");
                }
                else
                {
                    m_console.write("false");
                }
            }
            break;

            case entry_type::string:
            {
                char*       val         = reinterpret_cast<char*>(entry->value);
                const char* default_val = reinterpret_cast<const char*>(entry->default_value);
                m_console.write(val);
                m_console.write("\t");
                m_console.write(default_val);
            }
            break;

            default:
                break;
        }

        m_console.write_line("");
        entry++;
        id++;
    }
}

/** @brief Handler for the 'confwrite' command */
void config_console::confwrite_handler(const char* param_id)
{
    int                 id         = atoi(param_id);
    const config_entry* entry      = ov::config::get_desc();
    int                 current_id = -1;
    const char*         value      = m_console.get_next_param(param_id);
    if (value)
    {
        while (entry->name != nullptr)
        {
            current_id++;
            if (current_id == id)
            {
                break;
            }
            entry++;
        }
        if (current_id == id)
        {
            bool success = true;

            switch (entry->type)
            {
                case entry_type::sint:
                {
                    int      val       = atoi(value);
                    int32_t* entry_val = reinterpret_cast<int32_t*>(entry->value);
                    *entry_val         = static_cast<int32_t>(val);
                }
                break;

                case entry_type::uint:
                {
                    int       val       = atoi(value);
                    uint32_t* entry_val = reinterpret_cast<uint32_t*>(entry->value);
                    *entry_val          = static_cast<uint32_t>(val);
                }
                break;

                case entry_type::boolean:
                {
                    bool* entry_val = reinterpret_cast<bool*>(entry->value);
                    if (strcmp(value, "true") == 0)
                    {
                        *entry_val = true;
                    }
                    else if (strcmp(value, "false") == 0)
                    {
                        *entry_val = false;
                    }
                    else
                    {
                        success = false;
                    }
                }
                break;

                case entry_type::string:
                {
                    char* entry_val = reinterpret_cast<char*>(entry->value);
                    strncpy(entry_val, value, entry->size - 1u);
                    entry_val[entry->size - 1u] = 0;
                }
                break;

                default:
                    success = false;
                    break;
            }
            if (success)
            {
                m_console.write_line("Configuration value updated!");
            }
            else
            {
                m_console.write_line("Invalid value");
            }
        }
        else
        {
            m_console.write_line("Invalid parameter id");
        }
    }
    else
    {
        m_console.write_line("Missing parameter value");
    }
}

/** @brief Handler for the 'confsave' command */
void config_console::confsave_handler(const char*)
{
    if (ov::config::save())
    {
        m_console.write_line("Configuration saved!");
    }
    else
    {
        m_console.write_line("Error while saving configuration");
    }
}

/** @brief Handler for the 'confreset' command */
void config_console::confreset_handler(const char*)
{
    ov::config::set_default_values();
    m_console.write_line("Configuration reset to default values!");
}

} // namespace ov
