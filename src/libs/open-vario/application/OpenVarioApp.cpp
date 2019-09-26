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

#include "OpenVarioApp.h"

#include "OpenVarioConfig.h"

namespace open_vario
{


/** \brief Constructor */
OpenVarioApp::OpenVarioApp()
: m_os()
, m_board(m_config_manager)

, m_electronic_stamp_partition(m_board.config_eeprom(), 0u, 128u)       // 128 bytes
, m_boot_config_eeprom_partition(m_board.config_eeprom(), 128u, 128u)   // 128 bytes
, m_app_config_eeprom_partition(m_board.config_eeprom(), 256u, 3840u)   // 3840 bytes
, m_blackbox_eeprom_partition(m_board.config_eeprom(), 4096u, 4096u)    // 4096 bytes
                                                                        // => 8192 bytes used
, m_config_area_accessor(m_app_config_eeprom_partition, m_board.crc32())
, m_config_manager(OPEN_VARIO_CONFIG_VERSION, m_config_area_accessor)

, m_device_manager(m_config_manager, m_electronic_stamp_partition, m_board.crc32())

, m_log_manager(Log::LL_INFO, m_os)
, m_log_history(m_log_manager)

, m_mode_manager(m_operating_modes)
, m_operating_modes()
, m_init_mode(m_mode_manager, m_hmi_manager, m_time_manager, m_blackbox, m_device_manager, m_config_manager, m_sensors_manager, m_profile_manager, m_flight_recorder, m_ble_manager)
, m_run_mode(m_mode_manager, m_hmi_manager, m_sensors_manager, m_flight_recorder, m_ble_manager)
, m_power_off_mode(m_mode_manager, m_hmi_manager)

, m_hmi_manager(m_board.activityLed())

, m_time_manager(m_board.rtc())

, m_blackbox(m_time_manager, m_blackbox_eeprom_partition)

, m_altimeter(m_config_manager, m_board.altimeter())
, m_barometer(m_config_manager)
, m_thermometer(m_config_manager)
, m_variometer(m_config_manager)
, m_sensors_manager(m_config_manager, m_altimeter, m_barometer, m_thermometer, m_variometer)

, m_profile_manager(m_config_manager)

, m_flight_data_partition(m_board.flight_data_norflash(), 0u, (3u * m_board.flight_data_norflash().getSectorCount() / 4u))
, m_flight_data_fs(m_flight_data_partition)
, m_flight_recorder(m_config_manager, m_time_manager, m_profile_manager, m_flight_data_fs)

, m_ble_manager(m_config_manager, m_board.ble_stack(), m_device_manager)
{
    m_operating_modes.pushBack(&m_init_mode);
    m_operating_modes.pushBack(&m_run_mode);
    m_operating_modes.pushBack(&m_power_off_mode);
}

/** \brief Initialize the application */
bool OpenVarioApp::init(uint8_t argc, char* argv[])
{
    bool ret = false;

    // Initialize operating system
    if (m_os.init())
    {
        // Initialize application
        ret = onInit(argc, argv);
    }

    return ret;
}

/** \brief Start the application */
bool OpenVarioApp::start()
{
    bool ret = false;

    // Start application
    if (onStart())
    {
        // Start mode manager
        if (m_mode_manager.start())
        {
            // Start operating system
            ret = m_os.start();
        }
    }

    return ret;
}


}
