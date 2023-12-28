
#include "ov_config.h"
#include "fs.h"
#include "ov_config_default.h"

#include <cstring>

namespace ov
{

/** @brief Path to the configuration file */
static const char* OV_CONFIG_FILE_PATH = "/ov.cfg";

/** @brief Current configuration file version */
static const uint32_t CURRENT_CONFIG_VERSION = 0x00000001u;
/** @brief Magic number for start of configuration file */
static const uint32_t MAGIC_START = 0x8BADF00Du;
/** @brief Magic number for end of configuration file */
static const uint32_t MAGIC_END = 0xDEADBEEFu;

/** @brief Configuration */
static ov_config s_config;

/** @brief Configuration description */
static const config_entry s_config_desc[] = {
    // Device settings
    {"Device name", entry_type::string, sizeof(s_config.device_name), &s_config.device_name, s_default_device_name},
    {"Glider1 name", entry_type::string, sizeof(s_config.glider1_name), &s_config.glider1_name, s_default_glider1_name},
    {"Glider2 name", entry_type::string, sizeof(s_config.glider2_name), &s_config.glider2_name, s_default_glider2_name},
    {"Glider3 name", entry_type::string, sizeof(s_config.glider3_name), &s_config.glider3_name, s_default_glider3_name},
    {"Glider4 name", entry_type::string, sizeof(s_config.glider4_name), &s_config.glider4_name, s_default_glider4_name},
    // Sensors settings
    {"Sink rate integ time", entry_type::uint, sizeof(s_config.sr_integ_time), &s_config.sr_integ_time, &s_default_sr_integ_time},
    {"Glide ratio integ time", entry_type::uint, sizeof(s_config.gr_integ_time), &s_config.gr_integ_time, &s_default_gr_integ_time},
    // Display settings
    {"Night mode", entry_type::boolean, sizeof(s_config.is_night_mode_on), &s_config.is_night_mode_on, &s_default_is_night_mode_on},
    // Null entry
    {nullptr, entry_type::sint, 0u, nullptr, nullptr}};

namespace config
{

/** @brief Get the configuration */
ov_config& get()
{
    return s_config;
}

/** @brief Get the configuration description */
const config_entry* get_desc()
{
    return s_config_desc;
}

/** @brief Set the default configuration values */
void set_default_values()
{
    // For each configuration value
    auto* entry = &s_config_desc[0];
    while (entry->name != nullptr)
    {
        // Copy default value
        memcpy(entry->value, entry->default_value, entry->size);

        // Next entry
        entry++;
    }
}

/** @brief Load the configuration */
bool load()
{
    bool ret = false;

    // Open the configuration file
    auto file = ov::fs::open(OV_CONFIG_FILE_PATH, ov::fs::o_rdonly);
    if (file.is_open())
    {
        size_t   read_count = 0;
        uint32_t version    = 0;
        uint32_t magic      = 0;

        // Read header
        ret = file.read(&version, sizeof(version), read_count);
        ret = ret && file.read(&magic, sizeof(magic), read_count);
        ret = ret && (version == CURRENT_CONFIG_VERSION) && (magic == MAGIC_START);

        // Read values
        char       entry_name[64u];
        size_t     entry_name_len;
        entry_type type;
        uint16_t   entry_size;
        auto*      entry = &s_config_desc[0];
        while (ret && (entry->name != nullptr))
        {
            // Read entry
            ret = ret && file.read(&entry_name_len, sizeof(entry_name_len), read_count);
            ret = ret && (entry_name_len == strlen(entry->name));
            ret = ret && file.read(&entry_name, entry_name_len, read_count);
            ret = file.read(&type, sizeof(entry->type), read_count);
            ret = file.read(&entry_size, sizeof(entry->size), read_count);
            ret = ret && (type == entry->type) && (entry_size == entry->size);
            ret = ret && file.read(entry->value, entry->size, read_count);

            // Next entry
            entry++;
        }

        // Read footer
        ret = ret && file.read(&magic, sizeof(magic), read_count);
        ret = ret && (magic == MAGIC_END);
    }
    if (!ret)
    {
        // Set default values
        set_default_values();
    }

    return ret;
}

/** @brief Save the configuration */
bool save()
{
    bool ret = false;

    // Open the configuration file
    auto file = ov::fs::open(OV_CONFIG_FILE_PATH, ov::fs::o_creat | ov::fs::o_trunc | ov::fs::o_wronly);
    if (file.is_open())
    {
        size_t write_count = 0;

        // Write header
        ret = file.write(&CURRENT_CONFIG_VERSION, sizeof(CURRENT_CONFIG_VERSION), write_count);
        ret = ret && file.write(&MAGIC_START, sizeof(MAGIC_START), write_count);

        // Write values
        auto* entry = &s_config_desc[0];
        while (ret && (entry->name != nullptr))
        {
            // Write entry
            size_t entry_name_len = strlen(entry->name);

            ret = ret && file.write(&entry_name_len, sizeof(entry_name_len), write_count);
            ret = ret && file.write(&entry->name, entry_name_len, write_count);
            ret = ret && file.write(&entry->type, sizeof(entry->type), write_count);
            ret = ret && file.write(&entry->size, sizeof(entry->size), write_count);
            ret = ret && file.write(entry->value, entry->size, write_count);

            // Next entry
            entry++;
        }

        // Write footer
        ret = ret && file.write(&MAGIC_END, sizeof(MAGIC_END), write_count);
    }

    return ret;
}

} // namespace config
} // namespace ov
