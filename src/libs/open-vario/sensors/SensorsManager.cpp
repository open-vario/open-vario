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

#include "SensorsManager.h"
#include "OpenVarioConfig.h"
#include "IOpenVarioApp.h"
#include "LogMacro.h"


namespace open_vario
{


/** \brief Constructor */
SensorsManager::SensorsManager(ConfigManager& config_manager, Altimeter& altimeter, Barometer& barometer, Thermometer& thermometer, Variometer& variometer)
: m_config_manager(config_manager)
, m_altimeter(altimeter)
, m_barometer(barometer)
, m_thermometer(thermometer)
, m_variometer(variometer)

, m_config_values(OV_CONFIG_GROUP_SENSORS, "Sensors")
, m_config_acq_period(OV_CONFIG_VALUE_SENSOR_ACQ_PERIOD, "Acquisition period", 250u, 50u, 2000u, true)

, m_acq_period(0u)
, m_task("Sensors task", 5u)
, m_acq_timer()
, m_acq_trigger_sem(0u, 1u)
{
    // Register configuration values
    m_config_values.registerConfigValue(m_config_acq_period);
    m_config_manager.registerConfigValueGroup(m_config_values);
}

/** \brief Initialize the sensors manager */
bool SensorsManager::init()
{
    bool ret;

    LOG_INFO("Initializing sensors...");
    
    // Load configuration values
    ret = m_config_manager.getConfigValue<uint16_t>(OV_CONFIG_GROUP_SENSORS, OV_CONFIG_VALUE_SENSOR_ACQ_PERIOD, m_acq_period);
    if (ret)
    {
        // Initialize sensors
        ret = m_altimeter.init();
        ret = ret && m_barometer.init();
        ret = ret && m_thermometer.init();
        ret = ret && m_variometer.init();
    }

    return ret;
}

/** \brief Start the sensors manager */
bool SensorsManager::start()
{
    bool ret;

    LOG_INFO("Starting sensors acquisition...");

    // Start the acquisition timer
    ret = m_acq_timer.start(*this, m_acq_period);

    // Start the task
    ret = ret && m_task.start(*this, NULL);

    return ret;
}

/** \brief Start the sensors notifications */
bool SensorsManager::startNotifications()
{
    bool ret;

    LOG_INFO("Starting sensors notifications...");

    // Start notifications
    ret = m_altimeter.start();
    ret = ret && m_barometer.start();
    ret = ret && m_thermometer.start();
    ret = ret && m_variometer.start();

    return ret;
}

/** \brief Method which will be called at the task's startup */
void SensorsManager::taskStart(void* const param)
{
    (void)param;

    // Task loop
    while (true)
    {
        // Wait for acquisition trigger
        if (m_acq_trigger_sem.wait(IOpenVarioApp::getInstance().getOs().getInfiniteTimeoutValue()))
        {
            int32_t raw_altitude = 0;
            uint32_t raw_pressure = 0;
            int16_t raw_temperature = 0;
            
            // Compute altitude, pressure and temperature
            if (m_altimeter.compute(raw_altitude, raw_pressure, raw_temperature))
            {
                // Compute pressure
                if (!m_barometer.compute(raw_pressure))
                {
                    LOG_ERROR("Unable to compute pressure");    
                }

                // Compute temperature
                if (!m_thermometer.compute(raw_temperature))
                {
                    LOG_ERROR("Unable to compute temperature");    
                }

                // Compute vario
                int16_t raw_vario = 0;
                if (!m_variometer.compute(raw_altitude, raw_vario))
                {
                    LOG_ERROR("Unable to compute vario");    
                }

                LOG_INFO("Alti : %d, Pressure : %u, Temp : %d, Vario : %d", raw_altitude, raw_pressure, raw_temperature, raw_vario);
            }
            else
            {
                LOG_ERROR("Unable to compute altitude");
            }

            // Compute acceleration
            // TODO...
        }
    }
}

/** \brief Method which will be called when the timer elapsed */
void SensorsManager::timerElapsed(ITimer& timer)
{
    (void)timer;
    m_acq_trigger_sem.post(false);
}


}
