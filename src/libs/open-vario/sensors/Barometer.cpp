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

#include "Barometer.h"
#include "OpenVarioConfig.h"
#include "IOpenVarioApp.h"

namespace open_vario
{


/** \brief Constructor */
Barometer::Barometer(ConfigManager& config_manager)
: m_config_manager(config_manager)

, m_baro_filter()
, m_baro_values()

, m_config_values(OV_CONFIG_GROUP_BAROMETER, "Barometer")
, m_config_filter_depth(OV_CONFIG_VALUE_BARO_FILTER_DEPTH, "Raw value filter depth", 5u, 1u, m_baro_filter.getMaxFilterDepth(), false)

, m_started(false)

, m_listeners()
{
    // Register configuration values
    m_config_values.registerConfigValue(m_config_filter_depth);
    m_config_manager.registerConfigValueGroup(m_config_values);
}


/** \brief Initialize the barometer */
bool Barometer::init()
{
    // Load configuration values
    uint8_t filter_depth = 0u;
    const bool ret = m_config_manager.getConfigValue<uint8_t>(OV_CONFIG_GROUP_BAROMETER, OV_CONFIG_VALUE_BARO_FILTER_DEPTH, filter_depth);
    if (ret)
    {
        // Configure the filter
        m_baro_filter.setFilterDepth(filter_depth);

        // Initialize min and max values
        m_baro_values.min_pressure = 0xFFFFFFFFu;
        m_baro_values.max_pressure = 0;
    }

    return ret;
}

/** \brief Start the barometer */
bool Barometer::start()
{
    bool ret = false;

    if (!m_started)
    {
        m_started = true;
        ret = true;
    }

    return ret;
}

/** \brief Compute pressure value from raw pressure value */
bool Barometer::compute(const uint32_t raw_pressure)
{
    const bool ret = true;

    // Compute filtered value
    m_baro_values.pressure = m_baro_filter.compute(raw_pressure);

    // Update min and max values
    if (m_baro_values.pressure < m_baro_values.min_pressure)
    {
        m_baro_values.min_pressure = m_baro_values.pressure;
    }
    if (m_baro_values.pressure > m_baro_values.max_pressure)
    {
        m_baro_values.max_pressure = m_baro_values.pressure;
    }

    // Notify current value
    if (m_started)
    {
        for (nano_stl::nano_stl_size_t i = 0; i < m_listeners.getCount(); i++)
        {
            m_listeners[i]->onBarometerValues(m_baro_values);
        }
    }

    return ret;
}

/** \brief Register a listener for the barometer values */
bool Barometer::registerListener(IBarometerListener& listener)
{
    const bool ret = m_listeners.pushBack(&listener);
    return ret;
}

}
