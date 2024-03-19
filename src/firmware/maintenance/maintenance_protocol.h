/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_MAINTENANCE_PROTOCOL_H
#define OV_MAINTENANCE_PROTOCOL_H

#include <cstddef>
#include <cstdint>

namespace ov
{

// Forward declaration
class i_serial;

/** @brief Request identifiers */
enum class ov_request_id : uint8_t
{
    min, // Do not use
    device_infos,
    list_flights,
    list_flights_data,
    read_flight,
    read_flight_data,
    max // Do not use
};

/** @brief Request */
struct ov_request
{
    /** @brief Maximum payload size in bytes */
    static constexpr uint32_t MAX_PAYLOAD_SIZE = 5000u;

    /** @brief Identifier */
    ov_request_id id;
    /** @brief Size */
    uint16_t size;
    /** @brief Payload */
    uint8_t payload[MAX_PAYLOAD_SIZE];
};

/** @brief Maintenance protocol */
class maintenance_protocol
{
  public:
    /** @brief Constructor */
    maintenance_protocol(i_serial& serial_port);

    /** @brief Wait for an incoming request */
    ov_request& wait_for_request(uint32_t timeout);

    /** @brief Send a response */
    void send_response(const ov_request& request);

  protected:
    /** @brief Receive state */
    enum class rx_state : int
    {
        wait_sof1,
        wait_sof2,
        wait_sof3,
        wait_sof4,
        wait_id,
        wait_len1,
        wait_len2,
        wait_payload,
        wait_crc1,
        wait_crc2
    };

    /** @brief Serial port to use for the maintenance link */
    i_serial& m_serial_port;
    /** @brief Current request */
    ov_request m_request;

    /** @brief Inter bytes timeout in milliseconds */
    static constexpr uint32_t INTER_BYTES_TIMEOUT_MS = 500u;
    /** @brief Start of frame */
    static constexpr uint32_t START_OF_FRAME = 0x8BADF00Du;
    /** @brief Start of frame - 1st byte*/
    static constexpr uint8_t START_OF_FRAME_1 = 0x0Du;
    /** @brief Start of frame - 2nd byte*/
    static constexpr uint8_t START_OF_FRAME_2 = 0xF0u;
    /** @brief Start of frame - 3rd byte*/
    static constexpr uint8_t START_OF_FRAME_3 = 0xADu;
    /** @brief Start of frame - 4th byte*/
    static constexpr uint8_t START_OF_FRAME_4 = 0x8Bu;

    /** @brief Send data and update the CRC computation */
    bool send_and_update_crc(const void* data, size_t size, uint32_t& crc);
};

} // namespace ov

#endif // OV_MAINTENANCE_PROTOCOL_H
