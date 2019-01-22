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

#include "DeviceManager.h"
#include "LogMacro.h"
#include "OpenVarioConfig.h"
#include "nano-stl-conf.h"

namespace open_vario
{


/** \brief Constructor */
DeviceManager::DeviceManager(ConfigManager& config_manager, EepromPartition& eeprom_partition, ICrc32& crc32)
: m_config_manager(config_manager)
, m_eeprom_partition(eeprom_partition)
, m_crc32(crc32)

, m_config_values(OV_CONFIG_GROUP_DEVICE, "Device")
, m_config_device_name(OV_CONFIG_VALUE_DEVICE_NAME, "Device's name", "Open Vario Device", true)
{
    // Register configuration values
    m_config_values.registerConfigValue(m_config_device_name);
    m_config_manager.registerConfigValueGroup(m_config_values);
}


/** \brief Initialize the device manager */
bool DeviceManager::init()
{
    bool ret;

    LOG_INFO("Reading electronic stamp...");

    // Read the electronic stamp
    ret = m_eeprom_partition.read(0u, reinterpret_cast<uint8_t*>(&m_electronic_stamp), sizeof(ElectronicStamp));
    if (ret)
    {
        // Check electronic stamp integrity
        m_crc32.reset();
        m_crc32.update(&m_electronic_stamp, sizeof(ElectronicStamp) - sizeof(ElectronicStamp::crc));
        if ((m_electronic_stamp.magic_start != ElectronicStamp::START_MAGIC) ||
            (m_crc32.value() != m_electronic_stamp.crc))
        {
            ret = false;
            LOG_ERROR("Electronic stamp integrity check failed");
        }
    }
    else
    {
        LOG_ERROR("Unable to read electronic stamp");
    }
    if (ret)
    {
        LOG_INFO("Hw version : %s, Hw manufacturer : %s", m_electronic_stamp.hw_version, m_electronic_stamp.hw_manufacturer);
        LOG_INFO("Hw S/N : %s, Hw manufacturing date : %s", m_electronic_stamp.hw_serial_number, m_electronic_stamp.hw_manufacturing_date);
    }
    else
    {
        // Reset electronic stamp contents
        NANO_STL_MEMSET(&m_electronic_stamp, 0, sizeof(ElectronicStamp));
        NANO_STL_STRNCPY(m_electronic_stamp.hw_version, "Unknown", ElectronicStamp::HW_VERSION_MAX_SIZE);
        NANO_STL_STRNCPY(m_electronic_stamp.hw_manufacturer, "Unknown", ElectronicStamp::HW_MANUFACTURER_MAX_SIZE);
        NANO_STL_STRNCPY(m_electronic_stamp.hw_serial_number, "Unknown", ElectronicStamp::HW_SERIAL_NUMBER_MAX_SIZE);
        NANO_STL_STRNCPY(m_electronic_stamp.hw_manufacturing_date, "Unknown", ElectronicStamp::HW_MANUFACTURING_DATE_MAX_SIZE);
    }

    // Load configuration values
    ret = m_config_manager.getConfigValue(OV_CONFIG_GROUP_DEVICE, OV_CONFIG_VALUE_DEVICE_NAME, m_device_name);
    if (ret)
    {
        LOG_INFO("Device's name : %s", m_device_name);
    }

    return ret;
}

}
