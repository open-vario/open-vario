/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_BLE_SERVICE_H
#define OV_BLE_SERVICE_H

#include "i_ble_service.h"

namespace ov
{

/** @brief Interface for the BLE services implementations */
class ble_service : public i_ble_service
{
  public:
    /** @brief Constructor */
    ble_service(const char* name, const ble_uuid& uuid, i_ble_characteristic* chars[], size_t chars_count)
        : m_name(name), m_uuid(uuid), m_handle(0u), m_chars_count(chars_count), m_chars(chars)
    {
    }

    /** @brief Get the name of the attribute */
    const char* get_name() const override { return m_name; }

    /** @brief Get the unique identifier of the attribute */
    const ble_uuid& get_uuid() const override { return m_uuid; }

    /** @brief Get the handle of the attribute */
    uint16_t get_handle() const override { return m_handle; }

    /** @brief Set the handle of the attribute */
    void set_handle(uint16_t handle) override { m_handle = handle; }

    /** @brief Get the number of characteristics associated to the service */
    size_t get_chars_count() const override { return m_chars_count; }

    /** @brief Get the characteristics associated to the service */
    i_ble_characteristic** get_chars() override { return m_chars; }

  private:
    /** @brief Name of the service */
    const char* m_name;
    /** @brief Unique identifier of the service */
    const ble_uuid& m_uuid;
    /** @brief Handle of the service */
    uint16_t m_handle;
    /** @brief Number of characteristics associated to the service */
    size_t m_chars_count;
    /** @brief Characteristics associated to the service */
    i_ble_characteristic** m_chars;
};

} // namespace ov

#endif // OV_BLE_SERVICE_H
