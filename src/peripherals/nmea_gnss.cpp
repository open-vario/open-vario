

#include "nmea_gnss.h"
#include "os.h"

#include <cstdlib>
#include <cstring>

namespace ov
{

/** @brief Constructor */
nmea_gnss::nmea_gnss(i_serial& serial_port)
    : m_serial_port(serial_port),
      m_data{},
      m_frame_buffer{},
      m_cs_buffer{},
      m_last_received_byte_ts(0),
      m_timeout(INTER_FRAME_TIMEOUT),
      m_decoding_state(frame_decoding_state::wait_start),
      m_frame_size(0),
      m_checksum(0),
      m_checksum_size(0)
{
}

/** @brief Initialize the GNSS */
bool nmea_gnss::init()
{
    // Shall be overriden for specific implementations
    return true;
}

/** @brief Update navigation data */
bool nmea_gnss::update_data()
{
    bool ret            = true;
    bool byte_available = true;

    do
    {
        bool reset_state_machine = false;

        // Read available byte
        uint8_t byte;
        byte_available = m_serial_port.read(&byte, sizeof(byte), 0);
        if (byte_available)
        {
            // Save last received byte timestamp
            m_last_received_byte_ts = os::now();

            // Handle received byte
            switch (m_decoding_state)
            {
                case frame_decoding_state::wait_start:
                {
                    // Check start of frame char
                    if (byte == '$')
                    {
                        m_checksum       = 0u;
                        m_frame_size     = 0u;
                        m_timeout        = INTER_CHAR_TIMEOUT;
                        m_decoding_state = frame_decoding_state::wait_eof_cr;
                    }
                    break;
                }

                case frame_decoding_state::wait_eof_cr:
                {
                    // Check end of frame char
                    if (byte == '\r')
                    {
                        m_frame_buffer[m_frame_size] = 0u;
                        m_decoding_state             = frame_decoding_state::wait_eof_lf;
                    }
                    else if (byte == '*')
                    {
                        // The frame contains a checksum
                        m_checksum_size              = 0u;
                        m_frame_buffer[m_frame_size] = 0u;
                        m_decoding_state             = frame_decoding_state::wait_cs;
                    }
                    else
                    {
                        // Check frame length
                        if (m_frame_size == (sizeof(m_frame_buffer) - 1u))
                        {
                            // Frame is too long
                            reset_state_machine = true;
                        }
                        else
                        {
                            // Update checksum
                            m_checksum ^= byte;

                            // Convert separators to help decoding
                            if (byte == ',')
                            {
                                byte = 0u;
                            }

                            // Save received char
                            m_frame_buffer[m_frame_size] = byte;
                            m_frame_size++;
                        }
                    }
                    break;
                }

                case frame_decoding_state::wait_eof_lf:
                {
                    // Check end of frame char
                    if (byte == '\n')
                    {
                        // Decode frame
                        m_data.is_valid = decode_frame();
                    }

                    // Reset state machine
                    reset_state_machine = true;

                    break;
                }

                case frame_decoding_state::wait_cs:
                {
                    // Save received char
                    m_cs_buffer[m_checksum_size] = byte;
                    m_checksum_size++;
                    if (m_checksum_size == sizeof(m_cs_buffer))
                    {
                        // Verify checksum
                        const uint8_t received_checksum = convert_ndigits_int(m_cs_buffer, 2u, 16);
                        if (m_checksum == received_checksum)
                        {
                            // Wait for end of frame
                            m_decoding_state = frame_decoding_state::wait_eof_cr;
                        }
                        else
                        {
                            // Reset state machine
                            reset_state_machine = true;
                        }
                    }
                    break;
                }

                default:
                {
                    // Invalid state
                    reset_state_machine = true;
                    break;
                }
            }
        }
        else
        {
            // Check timeout
            if ((os::now() - m_last_received_byte_ts) >= m_timeout)
            {
                // Timeout, data is now invalid
                m_data              = {};
                m_decoding_state    = frame_decoding_state::wait_start;
                reset_state_machine = true;
            }
        }

        // Reset state machine if needed
        if (reset_state_machine)
        {
            m_decoding_state = frame_decoding_state::wait_start;
            m_timeout        = INTER_FRAME_TIMEOUT;
        }

    } while (byte_available);

    return ret;
}

/** @brief Decode the received frame */
bool nmea_gnss::decode_frame()
{
    bool data_valid = true;

    // Decode type
    const char*  frametype     = m_frame_buffer;
    const size_t frametype_len = strnlen(frametype, sizeof(m_frame_buffer));
    if (frametype_len > 3u)
    {
        // Extract type (skip receiver type)
        frametype += (frametype_len - 3u);

        // Decode data
        if (strncmp(frametype, "GGA", 4u) == 0u)
        {
            // GGA frame:  $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
            // - UTC time
            // - Latitude
            // - Latitude reference
            // - Longitude
            // - Longitude reference
            // - Fix quality
            // - Satellite count
            // - Horizontal dilution of precision
            // - Altitude
            // - Altitude unit
            // - Height of geoid
            // - Height of geoid unit
            // - Time in seconds since last DGPS update
            // - DGPS station ID number

            // Extract time
            const char* const time = get_next_frame_param(frametype);
            data_valid             = convert_time_param(time, m_data.date);

            // Extract latitude
            const char* const latitude = get_next_frame_param(time);
            data_valid                 = data_valid && convert_coordinates(latitude, m_data.latitude);

            // Skip reference
            const char* skip = get_next_frame_param(latitude);

            // Extract longitude
            const char* const longitude = get_next_frame_param(skip);
            data_valid                  = data_valid && convert_coordinates(longitude, m_data.longitude);

            // Skip reference
            skip = get_next_frame_param(longitude);

            // Skip fix quality
            const char* const fix_quality = get_next_frame_param(skip);
            data_valid                    = data_valid && (fix_quality[0] != '0');

            // Extract satellite count
            const char* const satellite_count = get_next_frame_param(fix_quality);
            m_data.satellite_count            = convert_ndigits_int(satellite_count, strnlen(satellite_count, 10u), 10u);

            // Skip horizontal dilution of precision
            skip = get_next_frame_param(satellite_count);

            // Extract altitude
            const char* const altitude = get_next_frame_param(skip);
            data_valid                 = data_valid && convert_altitude(altitude, m_data.altitude);
        }
        else if (strncmp(frametype, "RMC", 4u) == 0u)
        {
            // RMC frame : $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
            // - UTC time
            // - Status
            // - Latitude
            // - Latitude reference
            // - Longitude
            // - Longitude reference
            // - Speed
            // - Track angle
            // - Date
            // - Magnetic variation
            // - Magnetic variation reference

            // Extract time
            const char* const time = get_next_frame_param(frametype);
            data_valid             = convert_time_param(time, m_data.date);

            // Status
            const char* status = get_next_frame_param(time);
            data_valid         = data_valid && ((status[0] == 'A') || (status[0] == 'V'));

            // Extract latitude
            const char* const latitude = get_next_frame_param(status);
            data_valid                 = data_valid && convert_coordinates(latitude, m_data.latitude);

            // Reference
            const char* reference = get_next_frame_param(latitude);
            if (reference[0] == 'N')
            {
                // Keep positive sign
            }
            else if (reference[0] == 'N')
            {
                // Negative value
                m_data.latitude *= -1.;
            }
            else
            {
                // Invalid value
                data_valid = false;
            }

            // Extract longitude
            const char* const longitude = get_next_frame_param(reference);
            data_valid                  = data_valid && convert_coordinates(longitude, m_data.longitude);

            // Reference
            reference = get_next_frame_param(longitude);
            if (reference[0] == 'E')
            {
                // Keep positive sign
            }
            else if (reference[0] == 'W')
            {
                // Negative value
                m_data.longitude *= -1.;
            }
            else
            {
                // Invalid value
                data_valid = false;
            }

            // Extract speed
            const char* const speed = get_next_frame_param(reference);
            data_valid              = data_valid && convert_speed(speed, m_data.speed);

            // Skip track angle
            const char* const track_angle = get_next_frame_param(speed);
            data_valid                    = data_valid && convert_track_angle(track_angle, m_data.track_angle);

            // Extract date
            const char* const date = get_next_frame_param(track_angle);
            data_valid             = convert_date_param(date, m_data.date);
        }
        else
        {
            // Unknown frame => pass to child classes
            data_valid = handle_frame(frametype, get_next_frame_param(frametype));
        }
    }

    return data_valid;
}

/** @brief Get the next parameter in a NMEA frame */
const char* nmea_gnss::get_next_frame_param(const char* frame)
{
    while ((*frame) != 0)
    {
        frame++;
    }
    frame++;
    return frame;
}

/** @brief Convert a string representing a N digits unsigned integer to an integer */
uint32_t nmea_gnss::convert_ndigits_int(const char number[], const uint8_t digits, const uint8_t radix)
{
    uint32_t value = 0u;
    for (uint8_t i = 0; i < digits; i++)
    {
        value *= radix;
        if (number[i] > '9')
        {
            value += (15u - ('F' - number[i]));
        }
        else
        {
            value += (9u - ('9' - number[i]));
        }
    }
    return value;
}

/** @brief Convert a time parameter of a NMEA frame */
bool nmea_gnss::convert_time_param(const char* time_str, date_time& date)
{
    bool ret = false;
    if (strnlen(time_str, 10u) >= 6u)
    {
        date.hour   = convert_ndigits_int(&time_str[0u], 2u, 10u);
        date.minute = convert_ndigits_int(&time_str[2u], 2u, 10u);
        date.second = convert_ndigits_int(&time_str[4u], 2u, 10u);
        ret         = true;
    }
    return ret;
}

/** @brief Convert a date parameter of a NMEA frame */
bool nmea_gnss::convert_date_param(const char* date_str, date_time& date)
{
    bool ret = false;
    if (strnlen(date_str, 10u) == 6u)
    {
        date.day   = convert_ndigits_int(&date_str[0u], 2u, 10u);
        date.month = convert_ndigits_int(&date_str[2u], 2u, 10u);
        date.year  = convert_ndigits_int(&date_str[4u], 2u, 10u);
        ret        = true;
    }
    return ret;
}

/** @brief Convert a coordinates parameter of a NMEA frame */
bool nmea_gnss::convert_coordinates(const char* coordinates, double& coordinate)
{
    bool ret = true;

    // Extract coordinates and convert them in decimal degrees
    double   raw_coordinates = atof(coordinates);
    uint32_t degs            = static_cast<uint32_t>(raw_coordinates / 100.);
    double   mins            = raw_coordinates - static_cast<double>(degs * 100u);
    coordinate               = static_cast<double>(degs) + (mins / 60.);

    return ret;
}

/** @brief Convert an altitude parameter of a NMEA frame */
bool nmea_gnss::convert_altitude(const char* altitude, uint32_t& alti)
{
    bool ret = true;

    // Extract altitude
    double d_alti = atof(altitude);

    // Convert value
    alti = static_cast<uint32_t>(d_alti * 10.);

    return ret;
}

/** @brief Convert a speed parameter of a NMEA frame */
bool nmea_gnss::convert_speed(const char* speed, uint32_t& spd)
{
    bool ret = true;

    // Extract speed
    double d_speed = atof(speed);

    // Convert value
    spd = static_cast<uint32_t>(d_speed * 0.514444);

    return ret;
}

/** @brief Convert a track angle parameter of a NMEA frame */
bool nmea_gnss::convert_track_angle(const char* track_angle, uint16_t& ta)
{
    bool ret = true;

    // Extract track_angle
    double d_track_angle = atof(track_angle);

    // Convert value
    ta = static_cast<uint16_t>(d_track_angle * 10.);

    return ret;
}

} // namespace ov
