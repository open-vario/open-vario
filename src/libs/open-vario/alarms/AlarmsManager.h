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

#ifndef ALARMSMANAGER_H
#define ALARMSMANAGER_H


#include "ConfigManager.h"
#include "MinMaxConfigValue.h"
#include "IAltimeter.h"
#include "IBarometer.h"
#include "IThermometer.h"
#include "IVariometer.h"
#include "Delegate.h"

namespace open_vario
{


/** \brief Alarms manager */
class AlarmsManager : public IConfigValueListener
{
    public:

        /** \brief Constructor */
        AlarmsManager(ConfigManager& config_manager);


        /** \brief Initialize the alarms manager */
        bool init();

        

        /** \brief Called when a configuration value has been modified */
        virtual void onConfigValueChange(const IConfigValue& config_value);


    private:

        /** \brief Configuration manager */
        ConfigManager& m_config_manager;

        /** \brief Configuration values */
        ConfigValueGroup<19u> m_config_values;
        /** \brief Configuration value : altitude alarm enabled */
        ConfigValue<bool> m_config_altitude_alarm_enabled;
        /** \brief Configuration value : low altitude alarm threshold */
        MinMaxConfigValue<int16_t> m_config_low_altitude_alarm_threshold;
        /** \brief Configuration value : high altitude alarm threshold */
        MinMaxConfigValue<int16_t> m_config_high_altitude_alarm_threshold;
        /** \brief Configuration value : altitude alarm hysteresis */
        MinMaxConfigValue<uint8_t> m_config_altitude_alarm_hysteresis;
        /** \brief Configuration value : temperature alarm enabled */
        ConfigValue<bool> m_config_temperature_alarm_enabled;
        /** \brief Configuration value : low temperature alarm threshold */
        MinMaxConfigValue<int16_t> m_config_low_temperature_alarm_threshold;
        /** \brief Configuration value : high temperature alarm threshold */
        MinMaxConfigValue<int16_t> m_config_high_temperature_alarm_threshold;
        /** \brief Configuration value : temperature alarm hysteresis */
        MinMaxConfigValue<uint8_t> m_config_temperature_alarm_hysteresis;
        /** \brief Configuration value : vario alarm enabled */
        ConfigValue<bool> m_config_vario_alarm_enabled;
        /** \brief Configuration value : low vario alarm threshold */
        MinMaxConfigValue<int16_t> m_config_low_vario_alarm_threshold;
        /** \brief Configuration value : high vario alarm threshold */
        MinMaxConfigValue<int16_t> m_config_high_vario_alarm_threshold;
        /** \brief Configuration value : vario alarm hysteresis */
        MinMaxConfigValue<uint8_t> m_config_vario_alarm_hysteresis;
        /** \brief Configuration value : speed alarm enabled */
        ConfigValue<bool> m_config_speed_alarm_enabled;
        /** \brief Configuration value : low speed alarm threshold */
        MinMaxConfigValue<uint16_t> m_config_low_speed_alarm_threshold;
        /** \brief Configuration value : high speed alarm threshold */
        MinMaxConfigValue<uint16_t> m_config_high_speed_alarm_threshold;
        /** \brief Configuration value : speed alarm hysteresis */
        MinMaxConfigValue<uint8_t> m_config_speed_alarm_hysteresis;
        /** \brief Configuration value : acceleration alarm enabled */
        ConfigValue<bool> m_config_acceleration_alarm_enabled;
        /** \brief Configuration value : acceleration alarm threshold */
        MinMaxConfigValue<uint8_t> m_config_acceleration_alarm_threshold;
        /** \brief Configuration value : acceleration alarm hysteresis */
        MinMaxConfigValue<uint8_t> m_config_acceleration_alarm_hysteresis;
        

        /** \brief Represents an alarm configuration */
        template <typename ValueType>
        struct AlarmConfig
        {
            /** \brief Enabled flag */
            bool enabled;
            /** \brief Low threshold */
            ValueType low_threshold;
            /** \brief High threshold */
            ValueType high_threshold;
            /** \brief Hysteresis */
            uint8_t hysteresis;

            /** \brief Indicate if the alarm is active */
            bool active;
        };

        /** \brief Altitude alarm configuration */
        AlarmConfig<int16_t> m_alti_alarm_config;

        /** \brief Temperature alarm configuration */
        AlarmConfig<int16_t> m_temp_alarm_config;

        /** \brief Vario alarm configuration */
        AlarmConfig<uint16_t> m_vario_alarm_config;

        /** \brief Speed alarm configuration */
        AlarmConfig<int16_t> m_speed_alarm_config;

        /** \brief Acceleration alarm configuration */
        AlarmConfig<uint8_t> m_acceleration_alarm_config;


        /** \brief Indicate if altitude alarm configuration must be reloaded */
        bool m_reload_alti_alarm_config;

        /** \brief Indicate if temperature alarm configuration must be reloaded */
        bool m_reload_temp_alarm_config;

        /** \brief Indicate if vario alarm configuration must be reloaded */
        bool m_reload_vario_alarm_config;

        /** \brief Indicate if speed alarm configuration must be reloaded */
        bool m_reload_speed_alarm_config;

        /** \brief Indicate if acceleration alarm configuration must be reloaded */
        bool m_reload_acceleration_alarm_config;


        /** \brief Event handler to receive altimeter notifications */
        nano_stl::IEvent<const AltimeterValues&>::EventHandlerM m_altimeter_evt_handler;

        /** \brief Event handler to receive thermometer notifications */
        nano_stl::IEvent<const ThermometerValues&>::EventHandlerM m_thermometer_evt_handler;

        /** \brief Event handler to receive variometer notifications */
        nano_stl::IEvent<const VariometerValues&>::EventHandlerM m_variometer_evt_handler;



        /** \brief Load an alarm configuration */
        template <typename ValueType>
        bool loadAlarmConfig(const uint16_t group, const uint16_t id, AlarmConfig<ValueType>& alarm_config, const bool has_low_threshold, const bool has_high_threshold, const bool register_listener)
        {
            bool ret;
            
            // Load configuration
            ret = m_config_manager.getConfigValue<bool>(group, id, alarm_config.enabled);
            if (has_low_threshold)
            {
                ret = ret && m_config_manager.getConfigValue<ValueType>(group, id + 1u, alarm_config.low_threshold);
            }
            if (has_high_threshold)
            {
                ret = ret && m_config_manager.getConfigValue<ValueType>(group, id + 2u, alarm_config.high_threshold);
            }
            ret = ret && m_config_manager.getConfigValue<uint8_t>(group, id + 3u, alarm_config.hysteresis);

            // Register listeners
            if (ret && register_listener)
            {
                ret = ret && m_config_manager.registerConfigValueListener(group, id, *this);
                if (has_low_threshold)
                {
                    ret = ret && m_config_manager.registerConfigValueListener(group, id + 1u, *this);
                }
                if (has_high_threshold)
                {
                    ret = ret && m_config_manager.registerConfigValueListener(group, id + 2u, *this);
                }
                ret = ret && m_config_manager.registerConfigValueListener(group, id + 3u, *this);
            }

            return ret;
        }

        /** \brief Compute new alarm state */
        template <typename ValueType>
        bool computeAlarmState(const AlarmConfig<ValueType>& alarm_config, const ValueType& value, const bool has_low_threshold, const bool has_high_threshold)
        {
            bool active = alarm_config.active;

            if (active)
            {
                if (has_low_threshold)
                {
                    active = (value <= (alarm_config.low_threshold + static_cast<ValueType>(alarm_config.hysteresis)));
                }
                if (!active && has_high_threshold)
                {
                    active = (value >= (alarm_config.high_threshold - static_cast<ValueType>(alarm_config.hysteresis)));
                }
            }
            else
            {
                if (has_low_threshold)
                {
                    active = (value <= alarm_config.low_threshold);
                }
                if (!active && has_high_threshold)
                {
                    active = (value >= alarm_config.high_threshold);
                }
            }

            return active;
        }




        /** \brief Called when new altimeter values have been computed */
        void onAltimeterValues(const AltimeterValues& alti_values);

        /** \brief Called when new thermometer values have been computed */
        void onThermometerValues(const ThermometerValues& temp_values);

        /** \brief Called when new variometer values have been computed */
        void onVariometerValues(const VariometerValues& vario_values);

};

}

#endif // ALARMSMANAGER_H
