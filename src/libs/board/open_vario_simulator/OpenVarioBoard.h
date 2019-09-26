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

#ifndef OPENVARIOBOARD_H
#define OPENVARIOBOARD_H

#include "IOpenVarioBoard.h"

#ifdef OS_WINDOWS
#include "WindowsCpu.h"
#include "WindowsRtc.h"
#endif // OS_WINDOWS
#ifdef OS_LINUX
#include "LinuxCpu.h"
#include "LinuxRtc.h"
#endif // OS_LINUX

#include "McuRtc.h"
#include "SoftCrc32.h"

#include "SimuLed.h"
#include "SimuUart.h"
#include "SimuEeprom.h"
#include "SimuNorFlash.h"
#include "SimuBarometricSensor.h"
#include "SimuTemperatureSensor.h"
#include "SimuBlePeripheralStack.h"
#include "SimuGnss.h"
#include "SimuBuzzer.h"
#include "SimuUsbDeviceCdc.h"
#include "Simulator.h"

#include "BarometricAltimeter.h"

namespace open_vario
{

class ConfigManager;

/** \brief Open vario simulated board */
class OpenVarioBoard : public IOpenVarioBoard
{
    public:

        /** \brief Constructor */
        OpenVarioBoard(ConfigManager& config_manager);


        /** \brief Configure the board low-level peripherals */
        virtual bool configure();

        /** \brief Start the board peripherals */
        virtual bool start();

        /** \brief Get the board's name */
        virtual const char* name() const { return "Open Vario 1.0"; }

        /** \brief Get the board's CPU */
        virtual ICpu& cpu() { return m_cpu; }

        /** \brief Get the board's RTC */
        virtual IRtc& rtc() { return m_rtc; }

        /** \brief Get the board's activity LED */
        virtual ILed& activityLed() { return m_activity_led; }

        /** \brief Get the board's debug UART */
        virtual IUart& debugUart() { return m_debug_uart; }

        /** \brief Get the board's CRC-32 unit */
        virtual ICrc32& crc32() { return m_crc32; }

        /** \brief Get the board's EEPROM */
        virtual IEeprom& config_eeprom() { return m_config_eeprom; }

        /** \brief Get the board's flight data NOR flash */
        virtual INorFlash& flight_data_norflash() { return m_flight_data_flash; }

        /** \brief Get the board's altimeter sensor */
        virtual IBarometricAltimeter& altimeter() { return m_alti_sensor; }

        /** \brief Get the board's Bluetooth Low Energy stack */
        virtual IBlePeripheralStack& ble_stack() { return m_ble_stack; }

        /** \brief Get the board's GNSS */
        virtual IGnss& gnss() { return m_gnss; }

        /** \brief Get the board's buzzer */
        virtual IBuzzer& buzzer() { return m_buzzer; }

        /** \brief Get the board's USB Device CDC */
        virtual IUsbDeviceCdc& usbd_cdc() { return m_usbd_cdc; }


    private:

        /** \brief Configuration manager : do not use before call to configure function */
        ConfigManager& m_config_manager;

        /** \brief Simulator */
        Simulator m_simulator;


        #ifdef OS_WINDOWS
        /** \brief CPU */
        WindowsCpu m_cpu;

        /** \brief RTC */
        WindowsRtc m_rtc_driver;
        #endif // OS_WINDOWS
        #ifdef OS_LINUX
        /** \brief CPU */
        LinuxCpu m_cpu;

        /** \brief RTC */
        LinuxRtc m_rtc_driver;
        #endif // OS_LINUX

        /** \brief RTC peripheral */
        McuRtc m_rtc;

        /** \brief Activity LED */
        SimuLed m_activity_led;

        /** \brief Debug UART */
        SimuUart m_debug_uart;

        /** \brief CRC-32 unit (software) */
        SoftCrc32 m_crc32;

        /** \brief Configuration EEPROM */
        SimuEeprom m_config_eeprom;

        /** \brief Flight data NOR flash */
        SimuNorFlash m_flight_data_flash;

        /** \brief Temperature sensor */
        SimuTemperatureSensor m_temp_sensor;

        /** \brief Barometric sensor */
        SimuBarometricSensor m_baro_sensor;

        /** \brief Barometric altimeter */
        BarometricAltimeter m_alti_sensor;

        /** \brief GNSS */
        SimuGnss m_gnss;

        /** \brief BLE stack */
        SimuBlePeripheralStack m_ble_stack;

        /** \brief Buzzer */
        SimuBuzzer m_buzzer;

        /** \brief USB device CDC */
        SimuUsbDeviceCdc m_usbd_cdc;
};

}

#endif // OPENVARIOBOARD_H
