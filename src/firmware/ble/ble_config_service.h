
#ifndef OV_BLE_CONFIG_SERVICE_H
#define OV_BLE_CONFIG_SERVICE_H

#include "ble_characteristic.h"
#include "ble_service.h"

namespace ov
{

/** @brief BLE configuration service */
class ble_config_service
{
  public:
    /** @brief Constructor */
    ble_config_service();

    /** @brief Get the BLE service */
    i_ble_service& get_service() { return m_service; }

    /** @brief Set the initial values of the characteristics */
    void set_init_values();

  private:
    /** @brief Number of characteristics */
    static const size_t m_chars_count = 9u;
    /** @brief Characteristics */
    i_ble_characteristic* m_chars[m_chars_count];

    /** @brief BLE service */
    ble_service m_service;

    /** @brief Save config characteristic */
    ble_characteristic<uint32_t> m_save_config_char;
    /** @brief Device's name characteristic */
    ble_characteristic<char*> m_device_name_char;
    /** @brief Glider 1's name characteristic */
    ble_characteristic<char*> m_glider1_name_char;
    /** @brief Glider 2's name characteristic */
    ble_characteristic<char*> m_glider2_name_char;
    /** @brief Glider 3's name characteristic */
    ble_characteristic<char*> m_glider3_name_char;
    /** @brief Glider 4's name characteristic */
    ble_characteristic<char*> m_glider4_name_char;
    /** @brief Sink rate integration time characteristic */
    ble_characteristic<uint32_t> m_sr_integ_time_char;
    /** @brief Glide ratio integration time characteristic */
    ble_characteristic<uint32_t> m_gr_integ_time_char;
    /** @brief Night mode characteristic */
    ble_characteristic<bool> m_is_night_mode_on_char;

    /** @brief Event handler for the save config characteristic */
    bool save_config_handler(const uint32_t& new_value);
    /** @brief Event handler for the device's name characteristic */
    bool device_name_handler(const char* new_value);
    /** @brief Event handler for the glider 1's name characteristic */
    bool glider1_name_handler(const char* new_value);
    /** @brief Event handler for the glider 2's name characteristic */
    bool glider2_name_handler(const char* new_value);
    /** @brief Event handler for the glider 3's name characteristic */
    bool glider3_name_handler(const char* new_value);
    /** @brief Event handler for the glider 4's name characteristic */
    bool glider4_name_handler(const char* new_value);
    /** @brief Event handler for the sink rate integration time characteristic */
    bool sr_integ_time_char_handler(const uint32_t& new_value);
    /** @brief Event handler for the glide ratio integration time characteristic */
    bool gr_integ_time_char_handler(const uint32_t& new_value);
    /** @brief Event handler for the night mode characteristic */
    bool is_night_mode_on_char_handler(const bool& new_value);
};

} // namespace ov

#endif // OV_BLE_CONFIG_SERVICE_H
