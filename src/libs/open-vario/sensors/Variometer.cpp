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

#include "Variometer.h"
#include "OpenVarioConfig.h"
#include "IOpenVarioApp.h"

namespace open_vario
{

/** \brief Invalid altitude value */
#define INVALID_ALTITUDE_VALUE  -2147483648



/** \brief Constructor */
Variometer::Variometer(ConfigManager& config_manager)
: m_config_manager(config_manager)

, m_vario_filter()
, m_vario_values()

, m_config_values(OV_CONFIG_GROUP_VARIOMETER, "Variometer")
, m_config_filter_depth(OV_CONFIG_VALUE_VARIO_FILTER_DEPTH, "Raw value filter depth", 10u, 1u, m_vario_filter.getMaxFilterDepth(), false)

, m_started(false)

, m_previous_raw_altitude(INVALID_ALTITUDE_VALUE)
, m_previous_timestamp(0u)

, m_listeners()
{
    // Register configuration values
    m_config_values.registerConfigValue(m_config_filter_depth);
    m_config_manager.registerConfigValueGroup(m_config_values);
}


/** \brief Initialize the variometer */
bool Variometer::init()
{
    // Load configuration values
    uint8_t filter_depth = 0u;
    const bool ret = m_config_manager.getConfigValue<uint8_t>(OV_CONFIG_GROUP_VARIOMETER, OV_CONFIG_VALUE_VARIO_FILTER_DEPTH, filter_depth);
    if (ret)
    {
        // Configure the filter
        m_vario_filter.setFilterDepth(filter_depth);

        // Initialize min and max values
        m_vario_values.min_vario = 32767;
        m_vario_values.max_vario = -32768;
    }

    return ret;
}

/** \brief Start the variometer */
bool Variometer::start()
{
    bool ret = false;

    if (!m_started)
    {
        m_started = true;
        ret = true;
    }

    return ret;
}

/** \brief Compute vario value from raw altitude value */
bool Variometer::compute(const int32_t raw_altitude, int16_t& raw_vario)
{
    const bool ret = true;

    // Get timestamp
    const uint32_t timestamp = IOpenVarioApp::getInstance().getOs().getMsTimestamp(); 

    // Check if at least 1 altitude computation has been made
    if (m_previous_raw_altitude != INVALID_ALTITUDE_VALUE)
    { 
        // Compute raw value
        int32_t alti_diff = raw_altitude - m_previous_raw_altitude;
        uint32_t time_diff = timestamp - m_previous_timestamp;
        raw_vario = static_cast<int16_t>((alti_diff * 1000) / static_cast<int32_t>(time_diff));     

        // Compute filtered value
        m_vario_values.vario = m_vario_filter.compute(raw_vario);

        // Update min and max values
        if (m_vario_values.vario < m_vario_values.min_vario)
        {
            m_vario_values.min_vario = m_vario_values.vario;
        }
        if (m_vario_values.vario > m_vario_values.max_vario)
        {
            m_vario_values.max_vario = m_vario_values.vario;
        }

        // Notify current value
        if (m_started)
        {
            for (uint32_t i = 0; i < m_listeners.getCount(); i++)
            {
                m_listeners[i]->onVariometerValues(m_vario_values);
            }
        }
    }

    // Save values for next computation
    m_previous_raw_altitude = raw_altitude;
    m_previous_timestamp = timestamp;

    return ret;
}

/** \brief Register a listener for the variometer values */
bool Variometer::registerListener(IVariometerListener& listener)
{
    const bool ret = m_listeners.pushBack(&listener);
    return ret;
}

}
