/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_BLE_CHARACTERISTIC_H
#define OV_I_BLE_CHARACTERISTIC_H

#include "delegate.h"
#include "i_ble_attribute.h"

namespace ov
{

// Forward declaration
class i_ble_service;

/** @brief Interface for the BLE characteristics implementations */
class i_ble_characteristic : public i_ble_attribute
{
  public:
    /** 
     * @brief Delegate for characteristics events
     * @param characteristic Characteristic
     * @param new_value New value for the characteristic
     * @param new_size New size for the characteristic
     * @return true if the new value has been accepted, false otherwise
     */
    using event_handler = delegate<bool, i_ble_characteristic&, const void*, size_t>;

    /** @brief Properties */
    enum properties
    {
        /** @brief None */
        none = 0x00u,
        /** @brief Broadcast */
        broadcast = 0x01u,
        /** @brief Read */
        read = 0x02u,
        /** @brief Write without response */
        write_wo_resp = 0x04u,
        /** @brief Write */
        write = 0x08u,
        /** @brief Notify */
        notify = 0x10u,
        /** @brief Indicate */
        indicate = 0x20u,
        /** @brief Signed write */
        signed_write = 0x40u,
        /** @brief Ext */
        ext = 0x80u
    };

    /** @brief Destructor */
    virtual ~i_ble_characteristic() { }

    /** @brief Get the parent service of the characteristic */
    virtual i_ble_service& get_service() = 0;

    /** @brief Get the properties of the characteristic */
    virtual uint8_t get_properties() const = 0;

    /** @brief Get the size in bytes of the characteristic */
    virtual size_t get_size() const = 0;

    /** @brief Update the value of the characteristic from application */
    virtual bool update_value_from_app(const void* value, size_t new_size) = 0;

    /** @brief Update the value of the characteristic from stack */
    virtual bool update_value_from_stack(const void* value, size_t new_size) = 0;

    /** @brief Register the application event handler */
    virtual void register_app_event_handler(const event_handler& handler) = 0;

    /** @brief Register the stack event handler */
    virtual void register_stack_event_handler(const event_handler& handler) = 0;
};

} // namespace ov

#endif // OV_I_BLE_CHARACTERISTIC_H
