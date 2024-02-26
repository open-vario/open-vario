
#include "ble_rt_data_service.h"
#include "ov_data.h"

#include <cmath>
#include <limits>

namespace ov
{

/** @brief UUID of the service */
static const uint8_t BLE_RT_DATA_SERVICE_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x66u, 0x00u, 0x01u};

/** @brief UUID of the latitude characteristic */
static const uint8_t LATITUDE_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x66u, 0x01u, 0x01u};

/** @brief UUID of the longitude characteristic */
static const uint8_t LONGITUDE_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x66u, 0x02u, 0x01u};

/** @brief UUID of the speed characteristic */
static const uint8_t SPEED_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x66u, 0x03u, 0x01u};

/** @brief UUID of the altitude characteristic */
static const uint8_t ALTITUDE_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x66u, 0x04u, 0x01u};

/** @brief UUID of the total acceleration characteristic */
static const uint8_t TOTAL_ACCEL_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x66u, 0x05u, 0x01u};

/** @brief UUID of the sink rate characteristic */
static const uint8_t SINK_RATE_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x66u, 0x06u, 0x01u};

/** @brief UUID of the glide ratio characteristic */
static const uint8_t GLIDE_RATIO_CHAR_UUID[] = {
    0x51u, 0x6Cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3Bu, 0xBBu, 0x95u, 0xB2u, 0xA1u, 0x6Fu, 0x66u, 0x07u, 0x01u};

/** @brief Constructor */
ble_rt_data_service::ble_rt_data_service()
    : m_service("Real-time data service", BLE_RT_DATA_SERVICE_UUID, m_chars, m_chars_count),
      m_latitude_char(
          m_service, "Latitude", LATITUDE_CHAR_UUID, i_ble_characteristic::properties::read | i_ble_characteristic::properties::notify),
      m_longitude_char(
          m_service, "Longitude", LONGITUDE_CHAR_UUID, i_ble_characteristic::properties::read | i_ble_characteristic::properties::notify),
      m_speed_char(m_service, "Speed", SPEED_CHAR_UUID, i_ble_characteristic::properties::read | i_ble_characteristic::properties::notify),
      m_altitude_char(
          m_service, "Altitude", ALTITUDE_CHAR_UUID, i_ble_characteristic::properties::read | i_ble_characteristic::properties::notify),
      m_total_accel_char(m_service,
                         "Total acceleration",
                         TOTAL_ACCEL_CHAR_UUID,
                         i_ble_characteristic::properties::read | i_ble_characteristic::properties::notify),
      m_sink_rate_char(
          m_service, "Sink rate", SINK_RATE_CHAR_UUID, i_ble_characteristic::properties::read | i_ble_characteristic::properties::notify),
      m_glide_ratio_char(m_service,
                         "Glide ratio",
                         GLIDE_RATIO_CHAR_UUID,
                         i_ble_characteristic::properties::read | i_ble_characteristic::properties::notify)
{
    // Fill characteristics array
    m_chars[0u] = &m_latitude_char;
    m_chars[1u] = &m_longitude_char;
    m_chars[2u] = &m_speed_char;
    m_chars[3u] = &m_altitude_char;
    m_chars[4u] = &m_total_accel_char;
    m_chars[5u] = &m_sink_rate_char;
    m_chars[6u] = &m_glide_ratio_char;
}

/** @brief Update the values of the characteristics */
void ble_rt_data_service::update_values()
{
    auto data = ov::data::get();

    // GNSS data
    if (data.gnss.is_valid)
    {
        m_latitude_char.update_value(data.gnss.latitude);
        m_longitude_char.update_value(data.gnss.longitude);
        m_speed_char.update_value(data.gnss.speed);
    }
    else
    {
        m_latitude_char.update_value(NAN);
        m_longitude_char.update_value(NAN);
        m_speed_char.update_value(std::numeric_limits<uint32_t>::max());
    }

    // Alti data
    if (data.altimeter.is_valid)
    {
        m_altitude_char.update_value(data.altimeter.altitude);
    }
    else
    {
        m_altitude_char.update_value(std::numeric_limits<int32_t>::max());
    }

    // Accelerometer data
    if (data.accelerometer.is_valid)
    {
        m_total_accel_char.update_value(data.accelerometer.total_accel);
    }
    else
    {
        m_total_accel_char.update_value(std::numeric_limits<int16_t>::max());
    }

    // Computed data
    m_sink_rate_char.update_value(data.sink_rate);
    m_glide_ratio_char.update_value(data.glide_ratio);
}

} // namespace ov
