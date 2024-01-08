

#ifndef OV_NMEA_GNSS_H
#define OV_NMEA_GNSS_H

#include "i_gnss.h"
#include "i_serial.h"

namespace ov
{

/** @brief GNSS using NMEA frames over serial port to provide navigation data */
class nmea_gnss : public i_gnss
{
  public:
    /** @brief Constructor */
    nmea_gnss(i_serial& serial_port);

    /** @brief Initialize the GNSS */
    bool init();

    /** @brief Update navigation data */
    bool update_data() override;

    /** @brief Get the current navigation data */
    data get_data() override { return m_data; }

  protected:
    /** @brief Handle non-standard NMEA frames */
    virtual bool handle_frame(const char* frametype, const char* param)
    {
        (void)frametype;
        (void)param;
        // Return true to indicate that data is valid
        return true;
    }

  private:
    /** @brief Frame decoding state */
    enum class frame_decoding_state
    {
        /** @brief Wait start of frame */
        wait_start,
        /** @brief Wait end of frame '\r' */
        wait_eof_cr,
        /** @brief Wait end of frame '\n' */
        wait_eof_lf,
        /** @brief Wait end of checksum */
        wait_cs
    };

    /** @brief GNSS serial port */
    i_serial& m_serial_port;
    /** @brief GNSS data */
    data m_data;

    /** @brief Buffer to store received frames */
    char m_frame_buffer[128u];
    /** @brief Buffer to store received checksums */
    char m_cs_buffer[2u];
    /** @brief Timestamp of the last received byte */
    uint32_t m_last_received_byte_ts;
    /** @brief Current timeout */
    uint32_t m_timeout;
    /** @brief Current decoding state */
    frame_decoding_state m_decoding_state;
    /** @brief Current frame size */
    uint32_t m_frame_size;
    /** @brief Current checksum value */
    uint8_t m_checksum;
    /** @brief Current checksum size */
    uint32_t m_checksum_size;

    /** @brief Inter-frame timeout in milliseconds */
    static const uint32_t INTER_FRAME_TIMEOUT = 1500u;
    /** @brief Inter char timeout in milliseconds */
    static const uint32_t INTER_CHAR_TIMEOUT = 100u;

    /** @brief Decode the received frame */
    bool decode_frame();

  protected:
    /** @brief Get the next parameter in a NMEA frame */
    static const char* get_next_frame_param(const char* frame);
    /** @brief Convert a string representing a N digits unsigned integer to an integer */
    static uint32_t convert_ndigits_int(const char number[], const uint8_t digits, const uint8_t radix);
    /** @brief Convert a time parameter of a NMEA frame */
    static bool convert_time_param(const char* time_str, date_time& date);
    /** @brief Convert a date parameter of a NMEA frame */
    static bool convert_date_param(const char* date_str, date_time& date);
    /** @brief Convert a coordinates parameter of a NMEA frame */
    static bool convert_coordinates(const char* coordinates, double& coordinate);
    /** @brief Convert an altitude parameter of a NMEA frame */
    static bool convert_altitude(const char* altitude, uint32_t& alti);
    /** @brief Convert a speed parameter of a NMEA frame */
    static bool convert_speed(const char* speed, uint32_t& spd);
    /** @brief Convert a track angle parameter of a NMEA frame */
    static bool convert_track_angle(const char* track_angle, uint16_t& ta);
};

} // namespace ov

#endif // OV_NMEA_GNSS_H
