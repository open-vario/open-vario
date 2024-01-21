
#include "maintenance_manager.h"
#include "flight_file.h"
#include "fs.h"
#include "i_flight_recorder.h"
#include "os.h"
#include "ov_config.h"

#include <cstdio>
#include <cstring>

namespace ov
{

/** @brief Constructor */
maintenance_manager::maintenance_manager(i_serial& serial_port) : m_protocol(serial_port), m_thread() { }

/** @brief Initialize the maintenance */
bool maintenance_manager::init()
{
    // Start recording thread
    auto thread_func = ov::thread_func::create<maintenance_manager, &maintenance_manager::thread_func>(*this);
    bool ret         = m_thread.start(thread_func, "Maintenance", 2u, nullptr);

    return ret;
}

/** @brief Maintenance thread */
void maintenance_manager::thread_func(void*)
{
    // Thread loop
    while (true)
    {
        // Wait a request
        ov_request& request = m_protocol.wait_for_request(os::infinite_timeout_value());

        // Handle request
        bool send_response = false;
        switch (request.id)
        {
            case ov_request_id::device_infos:
                send_response = handle_device_infos_req(request);
                break;

            case ov_request_id::list_flights:
                send_response = handle_list_flights_req(request);
                break;

            case ov_request_id::read_flight:
                send_response = handle_read_flight_req(request);
                break;

            default:
                // Timeout
                break;
        }

        // Send response
        if (send_response)
        {
            m_protocol.send_response(request);
        }
    }
}

/** @brief Write a date time structure into a request */
void maintenance_manager::write(ov_request& request, const date_time& date)
{
    write(request, date.year);
    write(request, date.month);
    write(request, date.day);
    write(request, date.hour);
    write(request, date.minute);
    write(request, date.second);
    write(request, date.millis);
}

/** @brief Write a null terminated string into a request */
void maintenance_manager::write(ov_request& request, const char* str)
{
    uint8_t len = strlen(str);
    write(request, len);
    write(request, str, len);
}

/** @brief Write a buffer into a request */
void maintenance_manager::write(ov_request& request, const void* data, size_t size)
{
    memcpy(&request.payload[request.size], data, size);
    request.size += size;
}

/** @brief Handle the device infos request */
bool maintenance_manager::handle_device_infos_req(ov_request& request)
{
    // Prepare response
    request.size = 0;
    memset(request.payload, 0, sizeof(request.payload));

    write(request, ov::config::get().device_name);
    write(request, "stm32wb5mm-dk");
    write(request, OPENVARIO_MAJOR "." OPENVARIO_MINOR "." OPENVARIO_FIX);

    return true;
}

/** @brief Handle the list flights request */
bool maintenance_manager::handle_list_flights_req(ov_request& request)
{
    // Prepare response
    request.size = 0;
    memset(request.payload, 0, sizeof(request.payload));

    // Open the flights directory
    dir flight_dir = ov::fs::open_dir(i_flight_recorder::RECORDED_DATA_DIR);
    if (flight_dir.is_open())
    {
        // Send first response
        write(request, true);
        m_protocol.send_response(request);

        // List files
        dir::entry entry;
        bool       entry_found = true;
        while (entry_found && (m_protocol.wait_for_request(1000u).id == ov_request_id::list_flights_data))
        {
            // Go to next valid entry
            entry_found = false;
            while (!entry_found && flight_dir.read(entry))
            {
                if (entry.type == dir::entry_type::file)
                {
                    size_t filename_len = strlen(entry.name);
                    if ((filename_len > 4u) && (strcmp(&entry.name[filename_len - 4u], i_flight_recorder::RECORDED_DATA_EXT) == 0))
                    {
                        // Send response
                        request.size = 0;
                        memset(request.payload, 0, sizeof(request.payload));
                        write(request, true);
                        write(request, entry.name);
                        write(request, entry.size);
                        m_protocol.send_response(request);

                        entry_found = true;
                    }
                }
            }
        }

        // Last response is an empty response
        request.size = 0;
        memset(request.payload, 0, sizeof(request.payload));
        write(request, false);
    }
    else
    {
        // Error
        write(request, false);
    }

    return true;
}

/** @brief Handle the read flight request */
bool maintenance_manager::handle_read_flight_req(ov_request& request)
{
    char tmp[64u];
    snprintf(tmp, sizeof(tmp), "%s/%s", i_flight_recorder::RECORDED_DATA_DIR, reinterpret_cast<char*>(&request.payload[1u]));

    // Open the requested flight
    flight_file flight(tmp);
    if (flight.is_open())
    {
        // Send header
        const flight_file::header& header = flight.get_header();
        request.size                      = 0;
        memset(request.payload, 0, sizeof(request.payload));
        write(request, true);
        write(request, header.timestamp);
        write(request, header.glider);
        write(request, header.period);
        m_protocol.send_response(request);

        // Read file
        flight_file::entry entry;
        bool               entries_found = true;
        while (entries_found && (m_protocol.wait_for_request(1000u).id == ov_request_id::read_flight_data))
        {
            // Prepare response
            request.size = 0;
            memset(request.payload, 0, sizeof(request.payload));

            // Read a packet of entries
            int count = 0;
            while ((count < 25) && flight.read(entry))
            {
                // Write entry
                if (count == 0)
                {
                    write(request, true);
                }
                write(request, entry.gnss.is_valid);
                write(request, entry.gnss.latitude);
                write(request, entry.gnss.longitude);
                write(request, entry.gnss.altitude);
                write(request, entry.gnss.speed);
                write(request, entry.gnss.track_angle);
                write(request, entry.altimeter.is_valid);
                write(request, entry.altimeter.altitude);
                write(request, entry.altimeter.pressure);
                write(request, entry.altimeter.temperature);

                count++;
            }
            entries_found = (count != 0);
            if (entries_found)
            {
                // Send response
                m_protocol.send_response(request);
            }
        }

        // Last response is an empty response
        request.size = 0;
        memset(request.payload, 0, sizeof(request.payload));
        write(request, false);
    }
    else
    {
        // Error
        request.size = 0;
        memset(request.payload, 0, sizeof(request.payload));
        write(request, false);
    }

    return true;
}

} // namespace ov
