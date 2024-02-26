/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_MAINTENANCE_MANAGER_H
#define OV_MAINTENANCE_MANAGER_H

#include "maintenance_protocol.h"
#include "thread.h"

#include <type_traits>

namespace ov
{

// Forward declaration
struct date_time;

/** @brief Handle the maintenance link */
class maintenance_manager
{
  public:
    /** @brief Constructor */
    maintenance_manager(i_serial& serial_port);

    /** @brief Initialize the maintenance */
    bool init();

  protected:
    /** @brief Maintenance protocol */
    maintenance_protocol m_protocol;
    /** @brief Maintenance thread */
    thread<2048u> m_thread;

    /** @brief Maintenance thread */
    void thread_func(void*);

    /** @brief Write a date time structure into a request */
    void write(ov_request& request, const date_time& date);
    /** @brief Write a null terminated string into a request */
    void write(ov_request& request, const char* str);
    /** @brief Write a buffer into a request */
    void write(ov_request& request, const void* data, size_t size);
    /** @brief Write numeric value into a request */
    template <typename T, typename = typename std::enable_if_t<std::is_arithmetic<T>::value, T>>
    void write(ov_request& request, T value)
    {
        write(request, &value, sizeof(value));
    }

    /** @brief Handle the device infos request */
    bool handle_device_infos_req(ov_request& request);
    /** @brief Handle the list flights request */
    bool handle_list_flights_req(ov_request& request);
    /** @brief Handle the read flight request */
    bool handle_read_flight_req(ov_request& request);
};

} // namespace ov

#endif // OV_MAINTENANCE_MANAGER_H
