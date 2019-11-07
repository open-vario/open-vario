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

#include "IdentificationService.h"
#include "DeviceManager.h"
#include "IOpenVarioApp.h"

namespace open_vario
{


/** \brief Constructor */
IdentificationService::IdentificationService(DeviceManager& device_manager)
: OpenVarioBleServiceBase()
, m_device_manager(device_manager)

, m_identification_service("Identification service", "38df4da7-94f3-44dc-83ad-4e10864fbd44")

, m_command("Command", "520b42a8-ee29-46ec-9eff-24e732ca0cb5", true, IBleCharacteristic::PROP_WRITE)
, m_identification_info("Identification info", "dea233cc-dabb-4b00-9046-f70a44c1ceda", 32u, IBleCharacteristic::PROP_READ)

, m_cmd(CMD_RD_GATT_VERSION)
, m_cmd_available(true)
{}


/** \brief Initialize the BLE service */
bool IdentificationService::init()
{
    bool ret = true;

    // Fill BLE service with characteristics
    ret = ret && m_identification_service.addCharacteristic(m_command);
    ret = ret && m_identification_service.addCharacteristic(m_identification_info);

    return ret;
}

/** \brief Start the BLE service */
bool IdentificationService::start()
{
    bool ret = true;

    // Register to characteristics notifications
    ret = m_command.registerListener(*this);
    if (ret)
    {
        // Update characteristics values
        updateCharacteristicsValues();
    }

    return ret;
}

/** \brief Update the BLE service characteristics values */
void IdentificationService::updateCharacteristicsValues()
{
    // Check if a command is available
    if (m_cmd_available)
    {
        // Reset flag
        m_cmd_available = false;

        // Handle cmd
        switch (m_cmd)
        {
            case CMD_RD_GATT_VERSION:
            {
                m_identification_info.update("1.0");
                break;
            }

            case CMD_RD_SOFT_VERSION:
            {
                m_identification_info.update(IOpenVarioApp::getInstance().getVersion());
                break;
            }

            case CMD_RD_SOFT_MANUF_NAME:
            {
                m_identification_info.update(IOpenVarioApp::getInstance().getSwManufacturer());
                break;
            }

            case CMD_RD_HARD_VERSION:
            {
                m_identification_info.update(m_device_manager.getHwVersion());
                break;
            }

            case CMD_RD_HARD_MANUF_NAME:
            {
                m_identification_info.update(m_device_manager.getHwManufacturer());
                break;
            }

            case CMD_RD_HARD_SERIAL_NUMBER:
            {
                m_identification_info.update(m_device_manager.getHwSerialNumber());
                break;
            }

            case CMD_RD_HARD_MANUF_DATE:
            {
                m_identification_info.update(m_device_manager.getHwManufacturingDate());
                break;
            }

            default:
            {
                m_identification_info.update("Invalid command");
                break;
            }
        }
    }
}

/** \brief Called when the characteristic's value has changed */
void IdentificationService::onValueChanged(IBleCharacteristic& characteristic, const bool from_stack, const void* new_value, const uint16_t new_value_size)
{
    // Only handle modifications mades by the client
    if (from_stack)
    {
        // Save command
        if (new_value_size == m_command.valueLength())
        {
            m_cmd = *reinterpret_cast<const Command*>(new_value);
            m_cmd_available = true;
            triggerAsyncUpdate();
        }
    }
}

}
