/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_BLE_ATTRIBUTE_H
#define OV_I_BLE_ATTRIBUTE_H

#include <cstddef>
#include <cstdint>

namespace ov
{

/** @brief Unique identifier type => 128 bits UUID */
using ble_uuid = const uint8_t[16u];

/** @brief Interface for the BLE attibutes implementations */
class i_ble_attribute
{
  public:
    /** @brief Destructor */
    virtual ~i_ble_attribute() { }

    /** @brief Get the name of the attribute */
    virtual const char* get_name() const = 0;

    /** @brief Get the unique identifier of the attribute */
    virtual const ble_uuid& get_uuid() const = 0;

    /** @brief Get the handle of the attribute */
    virtual uint16_t get_handle() const = 0;

    /** @brief Set the handle of the attribute */
    virtual void set_handle(uint16_t handle) = 0;
};

} // namespace ov

#endif // OV_I_BLE_ATTRIBUTE_H
