/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "maintenance_protocol.h"
#include "i_serial.h"
#include "os.h"

#include <cstring>

namespace ov
{

/** @brief Constructor */
maintenance_protocol::maintenance_protocol(i_serial& serial_port) : m_serial_port(serial_port), m_request{} { }

/** @brief Wait for an incoming request */
ov_request& maintenance_protocol::wait_for_request(uint32_t timeout)
{
    // State machine env
    static const uint8_t START_OF_FRAME[] = {START_OF_FRAME_1, START_OF_FRAME_2, START_OF_FRAME_3, START_OF_FRAME_4};
    rx_state             state            = rx_state::wait_sof1;
    uint16_t             bytes_count      = 0;
    uint16_t             crc              = 0;
    uint32_t             expected_crc     = 0;
    bool                 reset_state      = true;
    uint8_t              byte             = 0;
    uint32_t             start_rx         = os::now();

    // Rx loop
    bool req_received = false;
    while (!req_received && ((os::now() - start_rx) < timeout))
    {
        // Reset state if needed
        if (reset_state)
        {
            state        = rx_state::wait_sof1;
            bytes_count  = 0;
            crc          = 0;
            expected_crc = 0;
            memset(&m_request, 0, sizeof(m_request));
            reset_state = false;
        }
        else
        {
            // Update CRC
            if (state <= rx_state::wait_crc1)
            {
                expected_crc += byte;
                expected_crc = expected_crc << 1;
            }
        }

        // Wait for an incoming byte
        if (m_serial_port.read(&byte, sizeof(byte), INTER_BYTES_TIMEOUT_MS))
        {
            // Handle received byte
            switch (state)
            {
                // Wait start of frame
                case rx_state::wait_sof1:
                    [[fallthrough]];
                case rx_state::wait_sof2:
                    [[fallthrough]];
                case rx_state::wait_sof3:
                    [[fallthrough]];
                case rx_state::wait_sof4:
                {
                    if (byte == START_OF_FRAME[bytes_count])
                    {
                        state = static_cast<rx_state>(static_cast<int>(state) + 1);
                        bytes_count++;
                    }
                    else
                    {
                        reset_state = true;
                    }
                }
                break;

                // Wait id
                case rx_state::wait_id:
                {
                    if ((byte > static_cast<uint8_t>(ov_request_id::min)) && (byte > static_cast<uint8_t>(ov_request_id::min)))
                    {
                        m_request.id = static_cast<ov_request_id>(byte);
                        state        = rx_state::wait_len1;
                    }
                    else
                    {
                        reset_state = true;
                    }
                }
                break;

                // Wait length
                case rx_state::wait_len1:
                {
                    m_request.size = byte;
                    state          = rx_state::wait_len2;
                }
                break;
                case rx_state::wait_len2:
                {
                    m_request.size += (byte << 8);
                    if (m_request.size == 0)
                    {
                        state = rx_state::wait_crc1;
                    }
                    else if (m_request.size <= ov_request::MAX_PAYLOAD_SIZE)
                    {
                        state       = rx_state::wait_payload;
                        bytes_count = 0;
                    }
                    else
                    {
                        reset_state = true;
                    }
                }
                break;

                // Wait payload
                case rx_state::wait_payload:
                {
                    m_request.payload[bytes_count] = byte;
                    bytes_count++;
                    if (bytes_count == m_request.size)
                    {
                        state = rx_state::wait_crc1;
                    }
                }
                break;

                // Wait CRC
                case rx_state::wait_crc1:
                {
                    crc   = byte;
                    state = rx_state::wait_crc2;
                }
                break;
                case rx_state::wait_crc2:
                {
                    crc += (byte << 8);
                    if (crc == static_cast<uint16_t>(expected_crc & 0x0000FFFFu))
                    {
                        req_received = true;
                    }
                    else
                    {
                        reset_state = true;
                    }
                }
                break;

                // Shall never happen ;)
                default:
                {
                    reset_state = true;
                }
                break;
            }
        }
        else
        {
            // Too much time between 2 bytes of the same frame
            reset_state = true;
        }
    }
    if (!req_received)
    {
        // Timeout, set invalid id
        m_request.id   = ov::ov_request_id::min;
        m_request.size = 0u;
    }

    return m_request;
}

/** @brief Send a response */
void maintenance_protocol::send_response(const ov_request& request)
{
    uint32_t crc = 0;

    // Send start of frame
    bool ret = send_and_update_crc(&START_OF_FRAME, sizeof(START_OF_FRAME), crc);

    // Send id and size
    ret = ret && send_and_update_crc(&request.id, sizeof(request.id), crc);
    ret = ret && send_and_update_crc(&request.size, sizeof(request.size), crc);

    // Send payload
    if (request.size != 0)
    {
        ret = ret && send_and_update_crc(request.payload, request.size, crc);
    }

    // Send CRC
    if (ret)
    {
        m_serial_port.write(&crc, sizeof(uint16_t));
    }
}

/** @brief Send data and update the CRC computation */
bool maintenance_protocol::send_and_update_crc(const void* data, size_t size, uint32_t& crc)
{
    // Send data
    bool ret = m_serial_port.write(data, size);

    // Update CRC
    const uint8_t* buff = reinterpret_cast<const uint8_t*>(data);
    while (size != 0)
    {
        crc += *buff;
        crc <<= 1;
        buff++;
        size--;
    }

    return ret;
}

} // namespace ov
