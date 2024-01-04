
#ifndef OV_CONFIG_DEFAULT_H
#define OV_CONFIG_DEFAULT_H

// Default configuration values

namespace ov
{

// Device settings

/** @brief Device's name */
static const char s_default_device_name[32u] = "OpenVarioDevice";
/** @brief Glider 1's name */
static const char s_default_glider1_name[32u] = "Glider 1";
/** @brief Glider 2's name */
static const char s_default_glider2_name[32u] = "Glider 2";
/** @brief Glider 3's name */
static const char s_default_glider3_name[32u] = "Glider 3";
/** @brief Glider 4's name */
static const char s_default_glider4_name[32u] = "Glider 4";

// Sensors settings

/** @brief Sink rate integration time (ms) */
static const uint32_t s_default_sr_integ_time = 2000u;
/** @brief Glide ratio integration time (ms) */
static const uint32_t s_default_gr_integ_time = 5000u;

// Display settings

/** @brief Night mode */
static const bool s_default_is_night_mode_on = true;

} // namespace ov

#endif // OV_CONFIG_DEFAULT_H