
#ifndef OV_CONFIG_H
#define OV_CONFIG_H

#include <cstdint>

namespace ov
{

/** @brief Open Vario configuration */
struct ov_config
{
    // Device settings

    /** @brief Device's name */
    char device_name[32u];
    /** @brief Glider 1's name */
    char glider1_name[32u];
    /** @brief Glider 2's name */
    char glider2_name[32u];
    /** @brief Glider 3's name */
    char glider3_name[32u];
    /** @brief Glider 4's name */
    char glider4_name[32u];
    /** @brief Selected glider */
    uint8_t glider;

    // Sensors settings

    /** @brief Sink rate integration time (ms) */
    uint32_t sr_integ_time;
    /** @brief Glide ratio integration time (ms) */
    uint32_t gr_integ_time;

    /** @brief Reference temperature for the barometric altimeter (1 = 0.1°C) */
    int16_t alti_ref_temp;
    /** @brief Reference pressure for the barometric altimeter (1 = 0.01mbar) */
    uint32_t alti_ref_pressure;
    /** @brief Reference altitude for the barometric altimeter (1 = 0.1m) */
    int32_t alti_ref_alti;

    // Recorder settings

    /** @brief Recording period in milliseconds */
    uint32_t recording_period;

    // Display settings

    /** @brief Night mode */
    bool is_night_mode_on;
};

/** @brief Confiuration entry type */
enum class entry_type : uint16_t
{
    /** @brief Signed integer */
    sint,
    /** @brief Unsigned integer */
    uint,
    /** @brief Boolean */
    boolean,
    /** @brief Single precision floating point */
    sfloat,
    /** @brief Double precision floating point */
    dfloat,
    /** @brief String */
    string
};

/** @brief Configuration entry */
struct config_entry
{
    /** @brief Name */
    const char* name;
    /** @brief Type */
    entry_type type;
    /** @brief Size */
    uint16_t size;
    /** @brief Value */
    void* value;
    /** @brief Default value */
    const void* default_value;
};

namespace config
{

/** @brief Get the configuration */
ov_config& get();

/** @brief Get the configuration description */
const config_entry* get_desc();

/** @brief Set the default configuration values */
void set_default_values();

/** @brief Load the configuration */
bool load();

/** @brief Save the configuration */
bool save();

} // namespace config
} // namespace ov

#endif // OV_CONFIG_H
