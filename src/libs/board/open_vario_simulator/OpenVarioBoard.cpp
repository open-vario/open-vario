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

#include "OpenVarioBoard.h"

namespace open_vario
{


/** \brief Constructor */
OpenVarioBoard::OpenVarioBoard(ConfigManager& config_manager)
: m_config_manager(config_manager)
, m_simulator(config_manager)

, m_cpu()
, m_rtc_driver()
, m_rtc(m_rtc_driver)
, m_activity_led(m_simulator, "activity_led", ILed::OFF)
, m_debug_uart(m_simulator, "debug_uart")
, m_crc32()
, m_config_eeprom("config_eeprom.bin", 32768u) // 32kB
, m_flight_data_flash("flight_data_norflash.bin", 8388608u, 4096u) // 8MB - 4kB

, m_temp_sensor(m_simulator, "temp_sensor")
, m_baro_sensor(m_simulator, "baro_sensor", m_temp_sensor)

, m_alti_sensor(m_baro_sensor)
{}

/** \brief Configure the board peripherals */
bool OpenVarioBoard::configure()
{
    bool ret = true;

    // Simulator
    ret = ret && m_simulator.configure();
    
    // RTC
    ret = ret && m_rtc.configure();

    // CRC-32
    ret = ret && m_crc32.configure();

    // Debug UART
    ret = ret && m_debug_uart.configure();

    // LEDs
    ret = ret && m_activity_led.configure();

    // EEPROM
    ret = ret && m_config_eeprom.configure();

    // NOR Flash
    ret = ret && m_flight_data_flash.configure();
   
    // Barometric sensor
    ret = ret && m_baro_sensor.configure();

    return ret;
}

/** \brief Start the board peripherals */
bool OpenVarioBoard::start()
{
    bool ret = true;

    // Simulator
    ret = ret && m_simulator.start();

    return ret;
}


}
