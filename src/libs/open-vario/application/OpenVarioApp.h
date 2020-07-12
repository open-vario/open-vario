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

#ifndef OPENVARIOAPP_H
#define OPENVARIOAPP_H

#include "IOpenVarioApp.h"
#include "Os.h"
#include "OpenVarioBoard.h"
#include "LogManager.h"
#include "LogHistory.h"
#include "EepromPartition.h"
#include "EepromConfigAreaAccessor.h"
#include "DeviceManager.h"
#include "ConfigManager.h"
#include "ModeManager.h"
#include "HmiManager.h"
#include "FaultManager.h"
#include "TimeManager.h"
#include "BlackBox.h"
#include "SensorsManager.h"
#include "ProfileManager.h"
#include "NorFlashPartition.h"
#include "NorFlashFs.h"
#include "FlightRecorder.h"
#include "BleManager.h"
#include "GnssManager.h"
#include "DiagnosticManager.h"
#include "InitMode.h"
#include "RunMode.h"
#include "DiagnosticMode.h"
#include "PowerOffMode.h"

#include "nano-stl.h"

namespace open_vario
{


/** \brief Base class for all open-vario applications implementations */
class OpenVarioApp : public IOpenVarioApp
{
    public:

        /** \brief Constructor */
        OpenVarioApp();


        /** \brief Get the version string */
        virtual const char* getVersion() { return "0.1"; }

        /** \brief Get the software manufacturer string */
        virtual const char* getSwManufacturer() { return "Open Vario"; }

        /** \brief Get the operating system */
        virtual IOs& getOs() { return m_os; }

        /** \brief Get the board */
        virtual IOpenVarioBoard& getBoard() { return m_board; }

        /** \brief Get the logger */
        virtual ILogger& getLogger() { return m_log_manager; }

        /** \brief Get the altimeter */
        virtual IAltimeter& getAltimeter() { return m_altimeter; }

        /** \brief Get the barometer */
        virtual IBarometer& getBarometer() { return m_barometer; }

        /** \brief Get the thermometer */
        virtual IThermometer& getThermometer() { return m_thermometer; }

        /** \brief Get the variometer */
        virtual IVariometer& getVariometer() { return m_variometer; }

        /** \brief Get the GNSS data provider */
        virtual IGnssProvider& getGnssProvider() { return m_gnss_manager; }


        /** \brief Initialize the application */
        virtual bool init(uint8_t argc, char* argv[]);

        /** \brief Start the application */
        virtual bool start();


    protected:

        /** \brief Called during application initialization */
        virtual bool onInit(uint8_t argc, char* argv[]) = 0;

        /** \brief Called before application start */
        virtual bool onStart() = 0;


        /** \brief Get the log history */
        virtual ILogHistory& getLogHistory() { return m_log_history; }

    private:

        /** \brief Operating system */
        Os m_os;

        /** \brief Board */
        OpenVarioBoard m_board;


        /** \brief Eeprom partition for electronic stamp */
        EepromPartition m_electronic_stamp_partition;

        /** \brief Eeprom partition for bootloader configuration */
        EepromPartition m_boot_config_eeprom_partition;

        /** \brief Eeprom partition for application configuration */
        EepromPartition m_app_config_eeprom_partition;

        /** \brief Eeprom partition for blackbox */
        EepromPartition m_blackbox_eeprom_partition;


        /** \brief Eeprom configuration area accessor */
        EepromConfigAreaAccessor m_config_area_accessor;
        
        /** \brief Configuration manager */
        ConfigManager m_config_manager;

        /** \brief Device manager */
        DeviceManager m_device_manager;
        
        /** \brief Log manager */
        LogManager m_log_manager;

        /** \brief Log history */
        LogHistory m_log_history;

        /** \brief Mode manager */
        ModeManager m_mode_manager;

        /** \brief Operating modes */
        nano_stl::StaticVector<IMode*, 3u> m_operating_modes;

        /** \brief Init mode */
        InitMode m_init_mode;

        /** \brief Run mode */
        RunMode m_run_mode;

        /** \brief Diagnostic mode */
        DiagnosticMode m_diag_mode;

        /** \brief Power off mode */
        PowerOffMode m_power_off_mode;

        /** \brief HMI manager */
        HmiManager m_hmi_manager;

        /** \brief Date and time manager */
        TimeManager m_time_manager;

        /** \brief Fault manager */
        FaultManager m_fault_manager;

        /** \brief Blackbox manager */
        BlackBox m_blackbox;

        /** \brief Altimeter */
        Altimeter m_altimeter;

        /** \brief Barometer */
        Barometer m_barometer;

        /** \brief Thermometer */
        Thermometer m_thermometer;

        /** \brief Variometer */
        Variometer m_variometer;

        /** \brief Sensors manager */
        SensorsManager m_sensors_manager;


        /** \brief Profile manager */
        ProfileManager m_profile_manager;

        
        /** \brief NOR Flash partition to store flight data */
        NorFlashPartition m_flight_data_partition;

        /** \brief File system to store fligh data files */
        NorFlashFs m_flight_data_fs;

        /** \brief Flight recorder */
        FlightRecorder m_flight_recorder;


        /** \brief Bluetooth Low Energy manager */
        BleManager m_ble_manager;


        /** \brief GNSS manager */
        GnssManager m_gnss_manager;

        /** \brief Diagnostic manager */
        DiagnosticManager m_diag_manager;


        /** \brief Delegate to receive fault events */
        IEvent<const FaultManager::Fault&>::EventHandlerM m_fault_delegate;

        /** \brief Called when a fault state has changed */
        void onFaultChanged(const FaultManager::Fault& fault);
};

}

#endif // OPENVARIOAPP_H
