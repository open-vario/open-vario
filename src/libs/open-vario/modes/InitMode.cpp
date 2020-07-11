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

#include "InitMode.h"
#include "ModeManager.h"
#include "HmiManager.h"
#include "TimeManager.h"
#include "FaultManager.h"
#include "BlackBox.h"
#include "DeviceManager.h"
#include "ConfigManager.h"
#include "SensorsManager.h"
#include "GnssManager.h"
#include "ProfileManager.h"
#include "FlightRecorder.h"
#include "BleManager.h"
#include "LogMacro.h"
#include "INorFlash.h"
#include "IGnss.h"
#include "IBuzzer.h"
#include "OpenVarioConfig.h"
#include "OpenVarioFaults.h"

namespace open_vario
{


/** \brief Constructor */
InitMode::InitMode(ModeManager& mode_manager, HmiManager& hmi_manager, TimeManager& time_manager, BlackBox& blackbox, DeviceManager& device_manager, 
                   ConfigManager& config_manager, SensorsManager& sensors_manager, GnssManager& gnss_manager, ProfileManager& profile_manager, 
                   FlightRecorder& flight_recorder, BleManager& ble_manager, FaultManager& fault_manager)
: m_mode_manager(mode_manager)
, m_hmi_manager(hmi_manager)
, m_time_manager(time_manager)
, m_black_box(blackbox)
, m_device_manager(device_manager)
, m_config_manager(config_manager)
, m_sensors_manager(sensors_manager)
, m_gnss_manager(gnss_manager)
, m_profile_manager(profile_manager)
, m_flight_recorder(flight_recorder)
, m_ble_manager(ble_manager)
, m_fault_manager(fault_manager)
{}

/** \brief Enter into the operating mode */
void InitMode::enter()
{
    // Initialize the board
    IOpenVarioBoard& board = IOpenVarioApp::getInstance().getBoard();
    bool ret = board.configure();
    if (!ret)
    {
        LOG_ERROR("Failure during board low-level initialization...");
    }
    else
    {
        LOG_INFO("Entering init mode...");

        // Start HMI
        m_hmi_manager.start();
        m_hmi_manager.getActivityLed().update(LedController::FAST_BLINK);

        // Initialize fault manager
        m_fault_manager.init();

        // Start date and time management
        if (!board.rtc().configure())
        {
            m_fault_manager.raise(OV_FAULT_RTC_KO);
        }
        m_time_manager.start();

        // Board autotest
        LOG_INFO("Starting level 1 autotests...");
        if (!autotest_l1())
        {
            LOG_ERROR("Autotests level 1 failed");
        }

        // Initialize blackbox
        if (!m_black_box.init())
        {
            LOG_ERROR("Unable to initialize blackbox");
        }

        // Initialize configuration
        m_config_manager.init();

        // Board autotest
        LOG_INFO("Starting level 2 autotests...");
        if (!autotest_l2())
        {
            LOG_ERROR("Autotests level 2 failed");
        }

        // Initialize device manager
        m_device_manager.init();

        // Start the board peripherals
        ret = board.start();
        if (ret)
        {
            // Initialize and start sensor manager
            ret = m_sensors_manager.init();
            ret = ret && m_sensors_manager.start();
            if (ret)
            {
                // Initialize profile manager
                ret = m_profile_manager.init();
                if (ret)
                {
                    // Initialize and start GNSS manager
                    ret = m_gnss_manager.init();
                    ret = ret && m_gnss_manager.start();
                    if (!ret)
                    {
                        LOG_ERROR("Failed to initialize GNSS manager");
                    }

                    // Initialize flight recorder
                    //ret = m_flight_recorder.init();
                    if (!ret)
                    {
                        LOG_ERROR("Failed to initialize flight recorder");
                    }
          
                    // Initialize BLE
                    ret = m_ble_manager.init();
                    if (!ret)
                    {
                        LOG_ERROR("Failed to initialize BLE");
                    }
                    
                    // Blink a bit :)
                    IOpenVarioApp::getInstance().getOs().waitMs(2000u);
                    IOpenVarioApp::getInstance().getOs().waitMs(12345678u);

                    ret = true;
                }
                else
                {
                    LOG_ERROR("Failed to initialize profile manager");
                }
            }
            else
            {
                LOG_ERROR("Failure during sensors startup");
            }
        }
        else
        {
            LOG_ERROR("Failure during board startup");
        }

        // Change mode
        if (ret)
        {
            // Init done, switch to run mode
            m_mode_manager.switchToMode(OPMODE_RUN);
        } 
        else
        {
            // Init failed, switch to power off mode
            m_mode_manager.switchToMode(OPMODE_POWEROFF);
        }
    }
}

/** \brief Leave the operating mode */
void InitMode::leave()
{
    LOG_INFO("Leaving init mode...");
}

/** \brief Start and check the board mandatory peripherals */
bool InitMode::autotest_l1()
{
    bool ret = true;
    IOpenVarioBoard& board = IOpenVarioApp::getInstance().getBoard();

    // Configuration EEPROM
    if (!board.config_eeprom().configure())
    {
        m_fault_manager.raise(OV_FAULT_CONFIG_EEPROM_KO);
        ret = false;
    }

    // Flight data NOR Flash
    if (!board.flight_data_norflash().configure())
    {
        m_fault_manager.raise(OV_FAULT_FLIGHT_DATA_FLASH_KO);
        ret = false;
    }

    // Altimeter
    IAltimeterSensor& alti = board.altimeter();
    if (!alti.configure())
    {
        m_fault_manager.raise(OV_FAULT_SENSOR_ALTI_KO);
        ret = false;
    }

    return ret;
}

/** \brief Start and check the board optional peripherals */
bool InitMode::autotest_l2()
{
    bool ret = true;
    IOpenVarioBoard& board = IOpenVarioApp::getInstance().getBoard();

    // Buzzer
    if (!board.buzzer().configure())
    {
        m_fault_manager.raise(OV_FAULT_BUZZER_KO);
        ret = false;
    }

    // GNSS
    bool gnss_enabled = false;
    m_config_manager.getConfigValue<bool>(OV_CONFIG_GROUP_GNSS, OV_CONFIG_VALUE_GNSS_ENABLED, gnss_enabled);
    if (gnss_enabled)
    {
        if (!board.gnss().probe())
        {
            m_fault_manager.raise(OV_FAULT_GNSS_KO);
            ret = false;
        }
    }

    // BLE
    bool ble_enabled = false;
    m_config_manager.getConfigValue<bool>(OV_CONFIG_GROUP_BLE, OV_CONFIG_VALUE_BLE_ENABLED, ble_enabled);
    if (ble_enabled)
    {
        if (!board.ble_stack().probe())
        {
            m_fault_manager.raise(OV_FAULT_BLE_KO);
            ret = false;
        }
    }

    return ret;
}

}
