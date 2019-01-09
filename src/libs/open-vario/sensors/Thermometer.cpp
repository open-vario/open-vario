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

#include "Thermometer.h"
#include "OpenVarioConfig.h"
#include "IOpenVarioApp.h"

namespace open_vario
{


/** \brief Constructor */
Thermometer::Thermometer(ConfigManager& config_manager)
: m_config_manager(config_manager)

, m_temp_values()
, m_temp_filter(m_config_manager, OV_CONFIG_GROUP_THERMOMETER, OV_CONFIG_VALUE_THERMO_FILTER_DEPTH, SensorFilter<int16_t, THERMO_FILTER_MAX_DEPTH>::SensorValues(m_temp_values.temperature, m_temp_values.min_temperature, m_temp_values.max_temperature))

, m_config_values(OV_CONFIG_GROUP_THERMOMETER, "Thermometer")
, m_config_filter_depth(OV_CONFIG_VALUE_THERMO_FILTER_DEPTH, "Raw value filter depth", 5u, 1u, THERMO_FILTER_MAX_DEPTH, false)

, m_started(false)

, m_thermometer_values_event()
{
    // Register configuration values
    m_config_values.registerConfigValue(m_config_filter_depth);
    m_config_manager.registerConfigValueGroup(m_config_values);
}


/** \brief Initialize the thermometer */
bool Thermometer::init()
{
    // Initialize filter
    const bool ret = m_temp_filter.init();
    return ret;
}

/** \brief Start the thermometer */
bool Thermometer::start()
{
    bool ret = false;

    if (!m_started)
    {
        m_started = true;
        ret = true;
    }

    return ret;
}

/** \brief Compute temperature value from raw temperature value */
bool Thermometer::compute(const int16_t raw_temperature)
{
    const bool ret = true;

    // Compute filtered value
    m_temp_filter.compute(raw_temperature);

    // Notify current value
    if (m_started)
    {
        m_thermometer_values_event.trigger(m_temp_values);
    }

    return ret;
}


}
