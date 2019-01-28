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

#include "BleManager.h"
#include "OpenVarioConfig.h"
#include "OpenVarioTasks.h"
#include "LogMacro.h"

namespace open_vario
{


/** \brief Constructor */
BleManager::BleManager(ConfigManager& config_manager, IBlePeripheralStack& ble_stack, DeviceManager& device_manager)
: m_config_manager(config_manager)
, m_ble_stack(ble_stack)

, m_config_values(OV_CONFIG_GROUP_BLE, "Bluetooth Low Energy")
, m_config_enabled(OV_CONFIG_VALUE_BLE_ENABLED, "BLE enabled", true, true)
, m_config_hw_address(OV_CONFIG_VALUE_BLE_HW_ADDRESS, "Hardware address", nullptr, true)
, m_config_update_period(OV_CONFIG_VALUE_BLE_UPDATE_PERIOD, "Update period", 500u, 50u, 1000u, true)

, m_enabled(false)
, m_update_period(0u)
, m_task("BLE manager", OV_TASK_PRIO_BLE_MANAGER)
, m_update_flag_set(0u)

, m_ble_services()

, m_identification_service(device_manager)
, m_flight_data_service()
{
    // Register configuration values
    m_config_values.registerConfigValue(m_config_enabled);
    m_config_values.registerConfigValue(m_config_hw_address);
    m_config_values.registerConfigValue(m_config_update_period);
    m_config_manager.registerConfigValueGroup(m_config_values);

    // Fill BLE services array
    m_ble_services.pushBack(&m_identification_service.getService());
    m_ble_services.pushBack(&m_flight_data_service.getService());
}


/** \brief Initialize the BLE manager */
bool BleManager::init()
{
    bool ret;

    LOG_INFO("Initializing BLE manager...");

    // Load configuration values
    IBlePeripheralStack::DeviceConfiguration device_config;
    ret = m_config_manager.getConfigValue<bool>(OV_CONFIG_GROUP_BLE, OV_CONFIG_VALUE_BLE_ENABLED, m_enabled);
    ret = ret && m_config_manager.getArrayConfigValue<uint8_t>(OV_CONFIG_GROUP_BLE, OV_CONFIG_VALUE_BLE_HW_ADDRESS, device_config.hw_address);
    ret = ret && m_config_manager.getStringConfigValue(OV_CONFIG_GROUP_DEVICE, OV_CONFIG_VALUE_DEVICE_NAME, device_config.name);
    ret = ret && m_config_manager.getConfigValue<uint16_t>(OV_CONFIG_GROUP_BLE, OV_CONFIG_VALUE_BLE_UPDATE_PERIOD, m_update_period);
    if (ret && m_enabled)
    {
        // Initialize the services
        ret = m_identification_service.init();
        ret = ret && m_flight_data_service.init();
        if (ret)
        {
            device_config.hw_address[0u] = 0x12u;
            device_config.hw_address[1u] = 0x34u;
            device_config.hw_address[2u] = 0x00u;
            device_config.hw_address[3u] = 0xE1u;
            device_config.hw_address[4u] = 0x80u;
            device_config.hw_address[5u] = 0x03u;
            
            // Initialize the BLE stack
            m_ble_stack.setListener(*this);
            ret = m_ble_stack.setDeviceConfiguration(device_config);
            ret = ret & m_ble_stack.setBleServices(m_ble_services);
        }
    }
    if (!m_enabled)
    {
        LOG_INFO("BLE disabled");
    }

    return ret;
}

/** \brief Start the BLE manager */
bool BleManager::start()
{
    bool ret = true;

    // Check if bluetooth is enabled
    if (m_enabled)
    {
        LOG_INFO("Starting BLE manager...");

        // Start the BLE stack
        ret = m_ble_stack.start();
        if (ret)
        {
            // Start the services
            ret = m_identification_service.start();
            ret = ret && m_flight_data_service.start();
            if (ret)
            {
                // Start the update timer
                ret = m_update_timer.start(*this, m_update_period);

                // Start the task
                ITask::TaskMethod task_method = TASK_METHOD(BleManager, task);
                ret = ret && m_task.start(task_method, nullptr);
            }
        }
    }

    return ret;
}

/** \brief Method which will be called when the timer elapsed */
void BleManager::timerElapsed(ITimer& timer)
{
    // Notify the task
    m_update_flag_set.set(FLAG_PERIODIC_UPDATE, false);
}

/** \brief Called when a client device is connected to the stack */
void BleManager::bleClientConnected()
{
    // Notify the task
    m_update_flag_set.set(FLAG_CLIENT_CONNECTED, false);
}

/** \brief Called when a client device is connected to the stack */
void BleManager::bleClientDisconnected()
{
    // Notify the task
    m_update_flag_set.set(FLAG_CLIENT_DISCONNECTED, false);
}

/** \brief BLE manager's task method */
void BleManager::task(void* const param)
{
    (void)param;

    // Task loop
    while (true)
    {
        // Wait for events
        uint32_t flag_mask = 0xFFFFFFFFu;
        const bool ret = m_update_flag_set.wait(flag_mask, true, IOs::getInstance().getInfiniteTimeoutValue());
        if (ret)
        {
            if ((flag_mask & FLAG_PERIODIC_UPDATE) != 0u)
            {
                // Periodic update
                m_identification_service.updateCharacteristicsValues();
                m_flight_data_service.updateCharacteristicsValues();
            }
            if ((flag_mask & FLAG_CLIENT_CONNECTED) != 0u)
            {
                // Client connected
                LOG_INFO("BLE client connected");
            }
            if ((flag_mask & FLAG_CLIENT_DISCONNECTED) != 0u)
            {
                // Client disconnected
                LOG_INFO("BLE client disconnected");
            }
        } 
    }
}

}
