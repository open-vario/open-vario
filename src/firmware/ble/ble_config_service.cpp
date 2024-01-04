
#include "ble_config_service.h"
#include "ov_config.h"

namespace ov
{

/** @brief UUID of the service */
static const uint8_t BLE_CONFIG_SERVICE_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x65u, 0x00u, 0x01u};

/** @brief UUID of the save config characteristic */
static const uint8_t SAVE_CONFIG_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x65u, 0x01u, 0x01u};

/** @brief UUID of the device's name characteristic */
static const uint8_t DEVICE_NAME_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x65u, 0x02u, 0x01u};

/** @brief UUID of the glider 1's name characteristic */
static const uint8_t GLIDER1_NAME_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x65u, 0x03u, 0x01u};

/** @brief UUID of the glider 2's name characteristic */
static const uint8_t GLIDER2_NAME_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x65u, 0x04u, 0x01u};

/** @brief UUID of the glider 3's name characteristic */
static const uint8_t GLIDER3_NAME_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x65u, 0x05u, 0x01u};

/** @brief UUID of the glider 4's name characteristic */
static const uint8_t GLIDER4_NAME_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x65u, 0x06u, 0x01u};

/** @brief UUID of the sink rate integration time characteristic */
static const uint8_t SINK_RATE_INTEG_TIME_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x65u, 0x07u, 0x01u};

/** @brief UUID of the glide ratio integration time characteristic */
static const uint8_t GLIDE_RATIO_INTEG_TIME_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x65u, 0x08u, 0x01u};

/** @brief UUID of the night mode characteristic */
static const uint8_t NIGHT_MODE_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x65u, 0x09u, 0x01u};

/** @brief Constructor */
ble_config_service::ble_config_service()
    : m_service("Configuration service", BLE_CONFIG_SERVICE_UUID, m_chars, m_chars_count),
      m_save_config_char(m_service, "Save config", SAVE_CONFIG_CHAR_UUID, i_ble_characteristic::properties::write),
      m_device_name_char(m_service,
                         "Device's name",
                         DEVICE_NAME_CHAR_UUID,
                         sizeof(ov_config::device_name),
                         i_ble_characteristic::properties::read | i_ble_characteristic::properties::write),
      m_glider1_name_char(m_service,
                          "Glider 1's name",
                          GLIDER1_NAME_CHAR_UUID,
                          sizeof(ov_config::glider1_name),
                          i_ble_characteristic::properties::read | i_ble_characteristic::properties::write),
      m_glider2_name_char(m_service,
                          "Glider 2's name",
                          GLIDER2_NAME_CHAR_UUID,
                          sizeof(ov_config::glider2_name),
                          i_ble_characteristic::properties::read | i_ble_characteristic::properties::write),
      m_glider3_name_char(m_service,
                          "Glider 3's name",
                          GLIDER3_NAME_CHAR_UUID,
                          sizeof(ov_config::glider3_name),
                          i_ble_characteristic::properties::read | i_ble_characteristic::properties::write),
      m_glider4_name_char(m_service,
                          "Glider 4's name",
                          GLIDER4_NAME_CHAR_UUID,
                          sizeof(ov_config::glider4_name),
                          i_ble_characteristic::properties::read | i_ble_characteristic::properties::write),
      m_sr_integ_time_char(m_service,
                           "Sink rate integ time",
                           SINK_RATE_INTEG_TIME_CHAR_UUID,
                           i_ble_characteristic::properties::read | i_ble_characteristic::properties::write),
      m_gr_integ_time_char(m_service,
                           "Glide ratio integ time",
                           GLIDE_RATIO_INTEG_TIME_CHAR_UUID,
                           i_ble_characteristic::properties::read | i_ble_characteristic::properties::write),
      m_is_night_mode_on_char(
          m_service, "Night mode", NIGHT_MODE_CHAR_UUID, i_ble_characteristic::properties::read | i_ble_characteristic::properties::write)
{
    // Fill characteristics array
    m_chars[0u] = &m_save_config_char;
    m_chars[1u] = &m_device_name_char;
    m_chars[2u] = &m_glider1_name_char;
    m_chars[3u] = &m_glider2_name_char;
    m_chars[4u] = &m_glider3_name_char;
    m_chars[5u] = &m_glider4_name_char;
    m_chars[6u] = &m_sr_integ_time_char;
    m_chars[7u] = &m_gr_integ_time_char;
    m_chars[8u] = &m_is_night_mode_on_char;

    // Register event handlers
    m_save_config_char.register_app_event_handler(TYPED_HANDLER(uint32_t, ble_config_service, save_config_handler, *this));
    m_device_name_char.register_app_event_handler(TYPED_HANDLER(char*, ble_config_service, device_name_handler, *this));
    m_glider1_name_char.register_app_event_handler(TYPED_HANDLER(char*, ble_config_service, glider1_name_handler, *this));
    m_glider2_name_char.register_app_event_handler(TYPED_HANDLER(char*, ble_config_service, glider2_name_handler, *this));
    m_glider3_name_char.register_app_event_handler(TYPED_HANDLER(char*, ble_config_service, glider3_name_handler, *this));
    m_glider4_name_char.register_app_event_handler(TYPED_HANDLER(char*, ble_config_service, glider4_name_handler, *this));
    m_sr_integ_time_char.register_app_event_handler(TYPED_HANDLER(uint32_t, ble_config_service, sr_integ_time_char_handler, *this));
    m_gr_integ_time_char.register_app_event_handler(TYPED_HANDLER(uint32_t, ble_config_service, gr_integ_time_char_handler, *this));
    m_is_night_mode_on_char.register_app_event_handler(TYPED_HANDLER(bool, ble_config_service, is_night_mode_on_char_handler, *this));
}

/** @brief Set the initial values of the characteristics */
void ble_config_service::set_init_values()
{
    auto& config = ov::config::get();
    m_device_name_char.update_value(config.device_name);
    m_glider1_name_char.update_value(config.glider1_name);
    m_glider2_name_char.update_value(config.glider2_name);
    m_glider3_name_char.update_value(config.glider3_name);
    m_glider4_name_char.update_value(config.glider4_name);
    m_sr_integ_time_char.update_value(config.sr_integ_time);
    m_gr_integ_time_char.update_value(config.gr_integ_time);
    m_is_night_mode_on_char.update_value(config.is_night_mode_on);
}

/** @brief Event handler for the save config characteristic */
bool ble_config_service::save_config_handler(const uint32_t& new_value)
{
    bool ret = false;
    if (new_value == 0x65766173u) // 'save' string
    {
        ret = ov::config::save();
    }
    if (new_value == 0x6B636162u) // 'back' string
    {
        ov::config::set_default_values();
        ret = true;
    }
    return ret;
}

/** @brief Event handler for the device's name characteristic */
bool ble_config_service::device_name_handler(const char* new_value)
{
    auto& config = ov::config::get();
    memset(config.device_name, 0, sizeof(config.device_name));
    strcpy(config.device_name, new_value);
    return true;
}

/** @brief Event handler for the glider 1's name characteristic */
bool ble_config_service::glider1_name_handler(const char* new_value)
{
    auto& config = ov::config::get();
    memset(config.glider1_name, 0, sizeof(config.glider1_name));
    strcpy(config.glider1_name, new_value);
    return true;
}

/** @brief Event handler for the glider 2's name characteristic */
bool ble_config_service::glider2_name_handler(const char* new_value)
{
    auto& config = ov::config::get();
    memset(config.glider2_name, 0, sizeof(config.glider2_name));
    strcpy(config.glider2_name, new_value);
    return true;
}

/** @brief Event handler for the glider 3's name characteristic */
bool ble_config_service::glider3_name_handler(const char* new_value)
{
    auto& config = ov::config::get();
    memset(config.glider3_name, 0, sizeof(config.glider3_name));
    strcpy(config.glider3_name, new_value);
    return true;
}

/** @brief Event handler for the glider 4's name characteristic */
bool ble_config_service::glider4_name_handler(const char* new_value)
{
    auto& config = ov::config::get();
    memset(config.glider4_name, 0, sizeof(config.glider4_name));
    strcpy(config.glider4_name, new_value);
    return true;
}

/** @brief Event handler for the sink rate integration time characteristic */
bool ble_config_service::sr_integ_time_char_handler(const uint32_t& new_value)
{
    auto& config         = ov::config::get();
    config.sr_integ_time = new_value;
    return true;
}

/** @brief Event handler for the glide ratio integration time characteristic */
bool ble_config_service::gr_integ_time_char_handler(const uint32_t& new_value)
{
    auto& config         = ov::config::get();
    config.gr_integ_time = new_value;
    return true;
}

/** @brief Event handler for the night mode characteristic */
bool ble_config_service::is_night_mode_on_char_handler(const bool& new_value)
{
    auto& config            = ov::config::get();
    config.is_night_mode_on = new_value;
    return true;
}

} // namespace ov
