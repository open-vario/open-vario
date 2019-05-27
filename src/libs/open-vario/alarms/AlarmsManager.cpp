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

#include "AlarmsManager.h"
#include "OpenVarioConfig.h"
#include "LogMacro.h"

namespace open_vario
{


/** \brief Constructor */
AlarmsManager::AlarmsManager(ConfigManager& config_manager)
: m_config_manager(config_manager)

, m_config_values(OV_CONFIG_GROUP_ALARMS, "Profile manager")
, m_config_altitude_alarm_enabled(OV_CONFIG_VALUE_ALARMS_ALTI_ENABLED, "Altitude alarm enabled", false, false)
, m_config_low_altitude_alarm_threshold(OV_CONFIG_VALUE_ALARMS_LALTI_THRESHOLD, "Low altitude alarm threshold", 0, -200, 10000, false)
, m_config_high_altitude_alarm_threshold(OV_CONFIG_VALUE_ALARMS_HALTI_THRESHOLD, "High altitude alarm threshold", 0, -200, 10000, false)
, m_config_altitude_alarm_hysteresis(OV_CONFIG_VALUE_ALARMS_ALTI_HYSTERESIS, "Altitude alarm hysteresys", 20u, 0u, 100u, false)

, m_config_temperature_alarm_enabled(OV_CONFIG_VALUE_ALARMS_TEMP_ENABLED, "Temperature alarm enabled", true, false)
, m_config_low_temperature_alarm_threshold(OV_CONFIG_VALUE_ALARMS_LTEMP_THRESHOLD, "Low temperature alarm threshold", 0, -600, 600, false)
, m_config_high_temperature_alarm_threshold(OV_CONFIG_VALUE_ALARMS_HTEMP_THRESHOLD, "High temperature alarm threshold", 450, -600, 600, false)
, m_config_temperature_alarm_hysteresis(OV_CONFIG_VALUE_ALARMS_TEMP_HYSTERESIS, "Temperature alarm hysteresis", 30u, 0u, 100u, false)

, m_config_vario_alarm_enabled(OV_CONFIG_VALUE_ALARMS_VARIO_ENABLED, "Vario alarm enabled", true, false)
, m_config_low_vario_alarm_threshold(OV_CONFIG_VALUE_ALARMS_LVARIO_THRESHOLD, "Low vario alarm threshold", -150, -300, 200, false)
, m_config_high_vario_alarm_threshold(OV_CONFIG_VALUE_ALARMS_HVARIO_THRESHOLD, "High vario alarm threshold", 100, -300, 200, false)
, m_config_vario_alarm_hysteresis(OV_CONFIG_VALUE_ALARMS_VARIO_HYSTERESIS, "Vario alarm hysteresis", 20u, 0u, 50u, false)

, m_config_speed_alarm_enabled(OV_CONFIG_VALUE_ALARMS_SPEED_ENABLED, "Speed alarm enabled", false, false)
, m_config_low_speed_alarm_threshold(OV_CONFIG_VALUE_ALARMS_LSPEED_THRESHOLD, "Low speed alarm threshold", 0u, 0u, 500u, false)
, m_config_high_speed_alarm_threshold(OV_CONFIG_VALUE_ALARMS_HSPEED_THRESHOLD, "High speed alarm threshold", 170u, 0u, 500u, false)
, m_config_speed_alarm_hysteresis(OV_CONFIG_VALUE_ALARMS_SPEED_HYSTERESIS, "Speed alarm hysteresis", 50u, 0u, 100u, false)

, m_config_acceleration_alarm_enabled(OV_CONFIG_VALUE_ALARMS_ACCEL_ENABLED, "Acceleration alarm enabled", true, false)
, m_config_acceleration_alarm_threshold(OV_CONFIG_VALUE_ALARMS_ACCEL_THRESHOLD, "Acceleration alarm threshold", 30u, 10u, 100u, false)
, m_config_acceleration_alarm_hysteresis(OV_CONFIG_VALUE_ALARMS_ACCEL_HYSTERESIS, "Acceleration alarm hysteresis", 5u, 0u, 10u, false)

, m_reload_alti_alarm_config(false)
, m_reload_temp_alarm_config(false)
, m_reload_vario_alarm_config(false)
, m_reload_speed_alarm_config(false)
, m_reload_acceleration_alarm_config(false)

, m_altimeter_evt_handler()
, m_thermometer_evt_handler()
, m_variometer_evt_handler()
{
    // Register configuration values
    m_config_values.registerConfigValue(m_config_altitude_alarm_enabled);
    m_config_values.registerConfigValue(m_config_low_altitude_alarm_threshold);
    m_config_values.registerConfigValue(m_config_high_altitude_alarm_threshold);
    m_config_values.registerConfigValue(m_config_altitude_alarm_hysteresis);
    m_config_values.registerConfigValue(m_config_temperature_alarm_enabled);
    m_config_values.registerConfigValue(m_config_low_temperature_alarm_threshold);
    m_config_values.registerConfigValue(m_config_high_temperature_alarm_threshold);
    m_config_values.registerConfigValue(m_config_temperature_alarm_hysteresis);
    m_config_values.registerConfigValue(m_config_vario_alarm_enabled);
    m_config_values.registerConfigValue(m_config_low_vario_alarm_threshold);
    m_config_values.registerConfigValue(m_config_high_vario_alarm_threshold);
    m_config_values.registerConfigValue(m_config_vario_alarm_hysteresis);
    m_config_values.registerConfigValue(m_config_speed_alarm_enabled);
    m_config_values.registerConfigValue(m_config_low_speed_alarm_threshold);
    m_config_values.registerConfigValue(m_config_high_speed_alarm_threshold);
    m_config_values.registerConfigValue(m_config_speed_alarm_hysteresis);
    m_config_values.registerConfigValue(m_config_acceleration_alarm_enabled);
    m_config_values.registerConfigValue(m_config_acceleration_alarm_threshold);
    m_config_values.registerConfigValue(m_config_acceleration_alarm_hysteresis);
    m_config_manager.registerConfigValueGroup(m_config_values);
}

/** \brief Initialize the alarms manager */
bool AlarmsManager::init()
{
    bool ret;

    LOG_INFO("Initializing alarm manager...");

    // Load configuration values
    ret = loadAlarmConfig<int16_t>(OV_CONFIG_GROUP_ALARMS, OV_CONFIG_VALUE_ALARMS_ALTI_ENABLED, m_alti_alarm_config, true, true, true);
    ret = loadAlarmConfig<int16_t>(OV_CONFIG_GROUP_ALARMS, OV_CONFIG_VALUE_ALARMS_TEMP_ENABLED, m_temp_alarm_config, true, true, true);
    ret = loadAlarmConfig<uint16_t>(OV_CONFIG_GROUP_ALARMS, OV_CONFIG_VALUE_ALARMS_VARIO_ENABLED, m_vario_alarm_config, true, true, true);
    ret = loadAlarmConfig<int16_t>(OV_CONFIG_GROUP_ALARMS, OV_CONFIG_VALUE_ALARMS_SPEED_ENABLED, m_speed_alarm_config, true, true, true);
    ret = loadAlarmConfig<uint8_t>(OV_CONFIG_GROUP_ALARMS, OV_CONFIG_VALUE_ALARMS_ACCEL_ENABLED, m_acceleration_alarm_config, false, true, true);
    if (ret)
    {
        // Register to sensor events
        m_altimeter_evt_handler = NANO_STL_EVENT_HANDLER_M(AlarmsManager, onAltimeterValues, const AltimeterValues&);
        ret = ret && IOpenVarioApp::getInstance().getAltimeter().altimeterValuesEvent().bind(m_altimeter_evt_handler);
        
        m_thermometer_evt_handler = NANO_STL_EVENT_HANDLER_M(AlarmsManager, onThermometerValues, const ThermometerValues&);
        ret = ret && IOpenVarioApp::getInstance().getThermometer().thermometerValuesEvent().bind(m_thermometer_evt_handler);
        
        m_variometer_evt_handler = NANO_STL_EVENT_HANDLER_M(AlarmsManager, onVariometerValues, const VariometerValues&);
        ret = ret && IOpenVarioApp::getInstance().getVariometer().variometerValuesEvent().bind(m_variometer_evt_handler);
        if (ret)
        {
            // Reset alarms
            m_alti_alarm_config.active = false;
            m_temp_alarm_config.active = false;
            m_vario_alarm_config.active = false;
            m_speed_alarm_config.active = false;
            m_acceleration_alarm_config.active = false;
        }
    }

    return ret;
}

/** \brief Called when a configuration value has been modified */
void AlarmsManager::onConfigValueChange(const IConfigValue& config_value)
{
    // Retrieve corresponding  alarm configuration
    const uint16_t id = config_value.id();
    if ((id >= OV_CONFIG_VALUE_ALARMS_ALTI_ENABLED) && (id <= OV_CONFIG_VALUE_ALARMS_ALTI_HYSTERESIS))
    {
        m_reload_alti_alarm_config = true;
    }
    else if ((id >= OV_CONFIG_VALUE_ALARMS_TEMP_ENABLED) && (id <= OV_CONFIG_VALUE_ALARMS_TEMP_HYSTERESIS))
    {
        m_reload_temp_alarm_config = true;
    }
    else if ((id >= OV_CONFIG_VALUE_ALARMS_VARIO_ENABLED) && (id <= OV_CONFIG_VALUE_ALARMS_VARIO_HYSTERESIS))
    {
        m_reload_vario_alarm_config = true;
    }
    else if ((id >= OV_CONFIG_VALUE_ALARMS_SPEED_ENABLED) && (id <= OV_CONFIG_VALUE_ALARMS_SPEED_HYSTERESIS))
    {
        m_reload_speed_alarm_config = true;
    }
    else if ((id >= OV_CONFIG_VALUE_ALARMS_ACCEL_ENABLED) && (id <= OV_CONFIG_VALUE_ALARMS_ACCEL_HYSTERESIS))
    {
        m_reload_acceleration_alarm_config = true;
    }
    else
    {
        // Ignore
    }
}

/** \brief Called when new altimeter values have been computed */
void AlarmsManager::onAltimeterValues(const AltimeterValues& alti_values)
{
    // Reload configuration
    if (m_reload_alti_alarm_config)
    {
        loadAlarmConfig<int16_t>(OV_CONFIG_GROUP_ALARMS, OV_CONFIG_VALUE_ALARMS_ALTI_ENABLED, m_alti_alarm_config, true, true, true);
    }
}

/** \brief Called when new thermometer values have been computed */
void AlarmsManager::onThermometerValues(const ThermometerValues& temp_values)
{

}

/** \brief Called when new variometer values have been computed */
void AlarmsManager::onVariometerValues(const VariometerValues& vario_values)
{

}

}
