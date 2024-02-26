/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_BLE_SERVICE_H
#define OV_I_BLE_SERVICE_H

#include "i_ble_attribute.h"
#include "i_ble_characteristic.h"

namespace ov
{

/** @brief Interface for the BLE services implementations */
class i_ble_service : public i_ble_attribute
{
  public:
    /** @brief Destructor */
    virtual ~i_ble_service() { }

    /** @brief Get the number of characteristics associated to the service */
    virtual size_t get_chars_count() const = 0;

    /** @brief Get the characteristics associated to the service */
    virtual i_ble_characteristic** get_chars() = 0;
};

} // namespace ov

#endif // OV_I_BLE_SERVICE_H
