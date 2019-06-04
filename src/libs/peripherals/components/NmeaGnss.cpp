/*
Copyright(c) 2017 Cedric Jimenez

This file is part of Open-Vario.

Open-Vario is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Open-Vario is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Open-Vario.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "IUart.h"
#include "NmeaGnss.h"
#include "nano-stl-conf.h"

#include <cstring>
#include <cstdlib>


namespace open_vario
{


/** \brief Constructor */
NmeaGnss::NmeaGnss(IUart& uart, ITask& rx_task)
: m_uart(uart)
, m_rx_task(rx_task)
, m_mutex()
, m_data_valid(false)
, m_nav_data()
, m_frame_buffer()
{}


/** \brief Read the current navigation data */
bool NmeaGnss::readData(NavigationData& nav_data)
{
    // Lock data
    m_mutex.lock();

    // Copy data
    bool ret = m_data_valid; 
    if (ret)
    {
        nav_data = m_nav_data;
    }

    // Unlock data
    m_mutex.unlock();

    return ret;
}

/** \brief Receive task method */
void NmeaGnss::receiveTask(void* const param)
{
    char cs[2u];
    uint8_t byte;
    uint8_t checksum;
    uint8_t cs_index;
    uint8_t frame_size;
    uint32_t timeout = INTER_FRAME_TIMEOUT;
    NmeaFrameDecodingState decoding_state = NFDS_WAIT_START;

    // Task loop
    while (true)
    {
        // Wait for a byte
        bool reset_state_machine = false;
        bool ret = m_uart.read(&byte, sizeof(byte), timeout);
        if (ret)
        {
            // Handle received byte
            switch (decoding_state)
            {
                case NFDS_WAIT_START:
                {
                    // Check start of frame char
                    if (byte == '$')
                    {
                        checksum = 0u;
                        frame_size = 0u;
                        timeout = INTER_CHAR_TIMEOUT;
                        decoding_state = NFDS_WAIT_EOF_CR;
                    }
                    break;
                }

                case NFDS_WAIT_EOF_CR:
                {
                    // Check end of frame char
                    if (byte == '\r')
                    {
                        m_frame_buffer[frame_size] = 0;
                        decoding_state = NFDS_WAIT_EOF_LF;
                    }
                    else if (byte == '*')
                    {
                        // The frame contains a checksum 
                        cs_index = 0u;
                        m_frame_buffer[frame_size] = 0;
                        decoding_state = NFDS_WAIT_CS;
                    }
                    else
                    {
                        // Check frame length
                        if (frame_size == (sizeof(m_frame_buffer) - 1u))
                        {
                            // Frame is too long
                            reset_state_machine = true;
                        }
                        else
                        {
                            // Update checksum
                            checksum ^= byte;

                            // Convert separators to help decoding
                            if (byte == ',')
                            {
                                byte = 0;
                            }

                            // Save received char
                            m_frame_buffer[frame_size] = byte;
                            frame_size++;
                        }                        
                    }
                    break;
                }

                case NFDS_WAIT_EOF_LF:
                {
                    // Check end of frame char
                    if (byte == '\n')
                    {
                        // Decode frame
                        decodeFrame();
                    }

                    // Reset state machine
                    reset_state_machine = true;

                    break;
                }

                case NFDS_WAIT_CS:
                {
                    // Save received char
                    cs[cs_index] = byte;
                    cs_index++;
                    if (cs_index == sizeof(cs))
                    {
                        // Verify checksum
                        const uint8_t received_checksum = convertNDigitsInt(cs, 2u, 16);
                        if (checksum == received_checksum)
                        {
                            // Wait for end of frame
                            decoding_state = NFDS_WAIT_EOF_CR;
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
            // Timeout, data is now invalid
            m_data_valid = false;

            reset_state_machine = true;
        }

        // Reset state machine if needed
        if (reset_state_machine)
        {
            decoding_state = NFDS_WAIT_START;
            timeout = INTER_FRAME_TIMEOUT;
        }
    }
}

/** \brief Start receiving NMEA frame */
bool NmeaGnss::startRx()
{
    // Start receive task
    ITask::TaskMethod task_method = TASK_METHOD(NmeaGnss, receiveTask);
    return m_rx_task.start(task_method, nullptr);
}

/** \brief Decode the received frame */
void NmeaGnss::decodeFrame()
{
    // Decode type
    const char* frametype = m_frame_buffer;
    const size_t frametype_len = NANO_STL_STRNLEN(frametype, sizeof(m_frame_buffer));
    if (frametype_len > 3u)
    {
        // Extract type (skip receiver type)
        frametype += (frametype_len - 3u);

        // Decode data
        if (NANO_STL_STRNCMP(frametype, "GGA", 4u) == 0u)
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
            const char* const time = getNextFrameParam(frametype);
            m_data_valid = convertTimeParam(time, m_nav_data.date_time);

            // Extract latitude
            const char* const latitude = getNextFrameParam(time);
            m_data_valid = m_data_valid && convertCoordinates(latitude, m_nav_data.latitude);

            // Skip reference
            const char* skip = getNextFrameParam(latitude);

            // Extract longitude
            const char* const longitude = getNextFrameParam(skip);
            m_data_valid = m_data_valid && convertCoordinates(longitude, m_nav_data.longitude);

            // Skip reference
            skip = getNextFrameParam(longitude);

            // Skip fix quality
            const char* const fix_quality = getNextFrameParam(skip);
            m_data_valid = m_data_valid && (fix_quality[0] != '0');

            // Extract satellite count
            const char* const satellite_count = getNextFrameParam(fix_quality);
            m_nav_data.satellite_count = convertNDigitsInt(satellite_count, NANO_STL_STRNLEN(satellite_count, 10u), 10u);

            // Skip horizontal dilution of precision
            skip = getNextFrameParam(satellite_count);

            // Extract altitude
            const char* const altitude = getNextFrameParam(skip);
            m_data_valid = m_data_valid && convertAltitude(altitude, m_nav_data.altitude);

        }
        else if (NANO_STL_STRNCMP(frametype, "RMC", 4u) == 0u)
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
            const char* const time = getNextFrameParam(frametype);
            m_data_valid = convertTimeParam(time, m_nav_data.date_time);

            // Status
            const char* status = getNextFrameParam(time);
            m_data_valid = m_data_valid && ((status[0] == 'A') || (status[0] == 'V'));

            // Extract latitude
            const char* const latitude = getNextFrameParam(status);
            m_data_valid = m_data_valid && convertCoordinates(latitude, m_nav_data.latitude);

            // Skip reference
            const char* skip = getNextFrameParam(latitude);

            // Extract longitude
            const char* const longitude = getNextFrameParam(skip);
            m_data_valid = m_data_valid && convertCoordinates(longitude, m_nav_data.longitude);

            // Skip reference
            skip = getNextFrameParam(longitude);

            // Extract speed
            const char* const speed = getNextFrameParam(skip);
            m_data_valid = m_data_valid && convertSpeed(speed, m_nav_data.speed);

            // Skip track angle
            skip = getNextFrameParam(speed);

            // Extract date
            const char* const date = getNextFrameParam(skip);
            m_data_valid = convertDateParam(date, m_nav_data.date_time);            
        }
        else
        {
            // Unknown frame
            handleRxFrame(frametype, getNextFrameParam(frametype));
        }
    }
}

/** \brief Get the next parameter in a NMEA frame */
const char* NmeaGnss::getNextFrameParam(const char* frame)
{
    while ((*frame) != 0)
    {
        frame++;
    }
    frame++;

    return frame;
}

/** \brief Convert a string representing a N digits unsigned integer to an integer */
uint32_t NmeaGnss::convertNDigitsInt(const char number[], const uint8_t digits, const uint8_t radix)
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

/** \brief Convert a time parameter of a NMEA frame */
bool NmeaGnss::convertTimeParam(const char* time, IRtc::DateTime& date_time)
{
    bool ret = false;
    if (NANO_STL_STRNLEN(time, 10u) >= 6)
    {
        date_time.hour = convertNDigitsInt(&time[0u], 2u, 10u);
        date_time.minute = convertNDigitsInt(&time[2u], 2u, 10u);
        date_time.second = convertNDigitsInt(&time[4u], 2u, 10u);
        ret = true;
    }
    return ret;
}

/** \brief Convert a date parameter of a NMEA frame */
bool NmeaGnss::convertDateParam(const char* date, IRtc::DateTime& date_time)
{
    bool ret = false;
    if (NANO_STL_STRNLEN(date, 10u) == 6)
    {
        date_time.day = convertNDigitsInt(&date[0u], 2u, 10u);
        date_time.month = convertNDigitsInt(&date[2u], 2u, 10u);
        date_time.year = convertNDigitsInt(&date[4u], 2u, 10u);
        ret = true;
    }
    return ret;
}

/** \brief Convert a coordinates parameter of a NMEA frame */
bool NmeaGnss::convertCoordinates(const char* coordinates, double& coordinate)
{
    bool ret = true;

    // Extract coordinates
    coordinate = NANO_STL_ATOF(coordinates);

    return ret;
}

/** \brief Convert an altitude parameter of a NMEA frame */
bool NmeaGnss::convertAltitude(const char* altitude, uint32_t& alti)
{
    bool ret = true;

    // Extract altitude
    double d_alti = NANO_STL_ATOF(altitude);

    // Convert value
    alti = static_cast<uint32_t>(d_alti * 10.);

    return ret;
}

/** \brief Convert a speed parameter of a NMEA frame */
bool NmeaGnss::convertSpeed(const char* speed, uint32_t& spd)
{
    bool ret = true;

    // Extract speed
    double d_speed = NANO_STL_ATOF(speed);

    // Convert value
    spd = static_cast<uint32_t>(d_speed * 0.514444);

    return ret;
}


}
