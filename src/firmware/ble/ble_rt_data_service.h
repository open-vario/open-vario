
#ifndef OV_BLE_RT_DATA_SERVICE_H
#define OV_BLE_RT_DATA_SERVICE_H

#include "ble_characteristic.h"
#include "ble_service.h"

namespace ov
{

/** @brief BLE real-time data service */
class ble_rt_data_service
{
  public:
    /** @brief Constructor */
    ble_rt_data_service();

    /** @brief Get the BLE service */
    i_ble_service& get_service() { return m_service; }

    /** @brief Update the values of the characteristics */
    void update_values();

  private:
    /** @brief Number of characteristics */
    static const size_t m_chars_count = 7u;
    /** @brief Characteristics */
    i_ble_characteristic* m_chars[m_chars_count];

    /** @brief BLE service */
    ble_service m_service;

    /** @brief Latitude characteristic */
    ble_characteristic<double> m_latitude_char;
    /** @brief Longitude characteristic */
    ble_characteristic<double> m_longitude_char;
    /** @brief Speed characteristic */
    ble_characteristic<uint32_t> m_speed_char;
    /** @brief Altitude characteristic */
    ble_characteristic<int32_t> m_altitude_char;
    /** @brief Total acceleration characteristic */
    ble_characteristic<int16_t> m_total_accel_char;
    /** @brief Sink rate characteristic characteristic */
    ble_characteristic<int16_t> m_sink_rate_char;
    /** @brief Glide ratio characteristic characteristic */
    ble_characteristic<uint16_t> m_glide_ratio_char;
};

} // namespace ov

#endif // OV_BLE_RT_DATA_SERVICE_H
