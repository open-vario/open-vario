/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_BLE_CHARACTERISTIC_H
#define OV_BLE_CHARACTERISTIC_H

#include "i_ble_characteristic.h"

#include <cstring>

namespace ov
{

/** @brief Base class for BLE characteristic implementations */
class ble_characteristic_base : public i_ble_characteristic
{
  public:
    /** @brief Constructor */
    ble_characteristic_base(i_ble_service& service, const char* name, const ble_uuid& uuid, size_t size, uint8_t properties);

    /** @brief Get the name of the attribute */
    const char* get_name() const override { return m_name; }

    /** @brief Get the unique identifier of the attribute */
    const ble_uuid& get_uuid() const override { return m_uuid; }

    /** @brief Get the handle of the attribute */
    uint16_t get_handle() const override { return m_handle; }

    /** @brief Set the handle of the attribute */
    void set_handle(uint16_t handle) override { m_handle = handle; }

    /** @brief Get the parent service of the characteristic */
    i_ble_service& get_service() override { return m_service; }

    /** @brief Get the properties of the characteristic */
    uint8_t get_properties() const override { return m_properties; }

    /** @brief Get the size in bytes of the characteristic */
    size_t get_size() const override { return m_size; }

    /** @brief Update the value of the characteristic from application */
    bool update_value_from_app(const void* value, size_t new_size) override;

    /** @brief Update the value of the characteristic from stack */
    bool update_value_from_stack(const void* value, size_t new_size) override;

    /** @brief Register the application event handler */
    void register_app_event_handler(const event_handler& handler) override { m_app_handler = handler; }

    /** @brief Register the stack event handler */
    void register_stack_event_handler(const event_handler& handler) override { m_stack_handler = handler; }

  private:
    /** @brief Parent BLE service */
    i_ble_service& m_service;
    /** @brief Name of the characteristic */
    const char* m_name;
    /** @brief Unique identifier of the characteristic */
    const ble_uuid& m_uuid;
    /** @brief Handle of the characteristic */
    uint16_t m_handle;
    /** @brief Properties of the characteristic */
    uint8_t m_properties;
    /** @brief Size in bytes of the characteristic */
    size_t m_size;
    /** @brief Application event handler */
    event_handler m_app_handler;
    /** @brief Stack event handler */
    event_handler m_stack_handler;
};

/** @brief BLE characteristic */
template <typename T>
class ble_characteristic : public ble_characteristic_base
{
  public:
    /** 
     * @brief Delegate for characteristics events 
     * @param new_value New value for the characteristic
     * @return true if the new value has been accepted, false otherwise
     */
    using typed_event_handler = delegate<bool, const T&>;

    /** @brief Constructor */
    ble_characteristic(i_ble_service& service, const char* name, const ble_uuid& uuid, uint8_t properties)
        : ble_characteristic_base(service, name, uuid, sizeof(T), properties)
    {
        // Reister application event handler
        ble_characteristic_base::register_app_event_handler(
            event_handler::create<ble_characteristic<T>, &ble_characteristic<T>::app_handler>(*this));
    }

    /** @brief Update the value of the characteristic */
    bool update_value(const T& value) { return ble_characteristic_base::update_value_from_app(&value, sizeof(T)); }

    /** @brief Register the application event handler */
    void register_app_event_handler(const typed_event_handler& handler) { m_app_handler = handler; }

  private:
    /** @brief Application event handler */
    typed_event_handler m_app_handler;

    /** @brief Application event handler*/
    bool app_handler(i_ble_characteristic&, const void* new_value, size_t new_size)
    {
        bool ret = false;

        // Check value and size
        if (new_value && (new_size == sizeof(T)))
        {
            // User handler
            if (!m_app_handler.is_null())
            {
                ret = m_app_handler.invoke(*reinterpret_cast<const T*>(new_value));
            }
        }

        return ret;
    }
};

/** @brief BLE string characteristic */
template <>
class ble_characteristic<char*> : public ble_characteristic_base
{
  public:
    /** 
     * @brief Delegate for characteristics events 
     * @param new_value New value for the characteristic
     * @return true if the new value has been accepted, false otherwise
     */
    using typed_event_handler = delegate<bool, const char*>;

    /** @brief Constructor */
    ble_characteristic(i_ble_service& service, const char* name, const ble_uuid& uuid, size_t size, uint8_t properties)
        : ble_characteristic_base(service, name, uuid, size - 1u, properties)
    {
        // Reister application event handler
        ble_characteristic_base::register_app_event_handler(
            event_handler::create<ble_characteristic<char*>, &ble_characteristic<char*>::app_handler>(*this));
    }

    /** @brief Update the value of the characteristic */
    bool update_value(const char* value) { return ble_characteristic_base::update_value_from_app(value, strnlen(value, get_size())); }

    /** @brief Register the application event handler */
    void register_app_event_handler(const typed_event_handler& handler) { m_app_handler = handler; }

  private:
    /** @brief Application event handler */
    typed_event_handler m_app_handler;

    /** @brief Application event handler*/
    bool app_handler(i_ble_characteristic&, const void* new_value, size_t new_size)
    {
        bool ret = false;

        // Check value and size
        if (new_value && (new_size <= get_size()))
        {
            // User handler
            if (!m_app_handler.is_null())
            {
                // Ensure that this will be a null-terminated string
                char* in_string     = reinterpret_cast<char*>(const_cast<void*>(new_value));
                in_string[new_size] = 0;
                ret                 = m_app_handler.invoke(const_cast<const char*>(in_string));
            }
        }

        return ret;
    }
};

/** @brief Create a typed event handler */
#define TYPED_HANDLER(type, class, method, inst) ble_characteristic<type>::typed_event_handler::create<class, &class ::method>(inst)

} // namespace ov

#endif // OV_BLE_CHARACTERISTIC_H
