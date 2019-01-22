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

#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "EepromPartition.h"
#include "ICrc32.h"
#include "ConfigManager.h"
#include "ConfigValue.h"


namespace open_vario
{


/** \brief Device manager */
class DeviceManager
{
    public:

        /** \brief Constructor */
        DeviceManager(ConfigManager& config_manager, EepromPartition& eeprom_partition, ICrc32& crc32);


        /** \brief Initialize the device manager */
        bool init();
        

        /** \brief Get the hardware version string */
        const char* getHwVersion() const { return m_electronic_stamp.hw_version; }

        /** \brief Get the hardware manufacturer string */
        const char* getHwManufacturer() const { return m_electronic_stamp.hw_manufacturer; }

        /** \brief Get the hardware serial number string */
        const char* getHwSerialNumber() const { return m_electronic_stamp.hw_serial_number; }

        /** \brief Get the hardware manufacturing date string */
        const char* getHwManufacturingDate() const { return m_electronic_stamp.hw_manufacturing_date; }

        /** \brief Get the device's name */
        const char* getDeviceName() const { return m_device_name; }
        

    private:


        /** \brief Electronic stamp data */
        struct ElectronicStamp
        {
            /** \brief Electronic stamp start magic : 'OVID' in ASCII */
            static const uint32_t START_MAGIC = 0x4449564F;

            /** \brief Maximum size of the hardware version string */
            static const uint32_t HW_VERSION_MAX_SIZE = 15u;

            /** \brief Maximum size of the hardware manufacturer string */
            static const uint32_t HW_MANUFACTURER_MAX_SIZE = 31u;

            /** \brief Maximum size of the hardware serial number string */
            static const uint32_t HW_SERIAL_NUMBER_MAX_SIZE = 31u;

            /** \brief Maximum size of the hardware manufacturing date string (format : YYYY/MM/DD-hh:mm:ss) */
            static const uint32_t HW_MANUFACTURING_DATE_MAX_SIZE = 19u;


            /** \brief Magic start */
            uint32_t magic_start;

            /** \brief Hardware version */
            char hw_version[HW_VERSION_MAX_SIZE + 1u];

            /** \brief Hardware manufacturer name */
            char hw_manufacturer[HW_MANUFACTURER_MAX_SIZE + 1u];

            /** \brief Hardware serial number name */
            char hw_serial_number[HW_SERIAL_NUMBER_MAX_SIZE + 1u];

            /** \brief Manufacturing date */
            char hw_manufacturing_date[HW_MANUFACTURING_DATE_MAX_SIZE + 1u];

            /** \brief Reserved */
            uint8_t reserved[20u];

            /** \brief CRC-32 */
            uint32_t crc;
        };

        /** \brief Maximum size of device's name string */
        static const uint8_t MAX_DEVICE_NAME_SIZE = 31u;


        /** \brief Configuration manager */
        ConfigManager& m_config_manager;

        /** \brief EEPROM partition */
        EepromPartition& m_eeprom_partition;

        /** \brief CRC-32 */
        ICrc32& m_crc32;

        /** \brief Configuration values */
        ConfigValueGroup<1u> m_config_values;
        /** \brief Configuration value : device's name */
        StringConfigValue<MAX_DEVICE_NAME_SIZE> m_config_device_name;


        /** \brief Electronic stamp */
        ElectronicStamp m_electronic_stamp;

        /** \brief Device's name */
        char m_device_name[MAX_DEVICE_NAME_SIZE + 1u];
};

}

#endif // DEVICEMANAGER_H
