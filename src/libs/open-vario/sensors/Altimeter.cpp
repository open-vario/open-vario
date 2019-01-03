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

#include "Altimeter.h"
#include "OpenVarioConfig.h"


namespace open_vario
{


/** \brief Constructor */
Altimeter::Altimeter(ConfigManager& config_manager, IBarometricAltimeter& barometric_altimeter)
: m_config_manager(config_manager)
, m_barometric_altimeter(barometric_altimeter)

, m_alti_values()
, m_alti_filter(config_manager, OV_CONFIG_GROUP_ALTIMETER, OV_CONFIG_VALUE_ALTI_FILTER_DEPTH, SensorFilter<int32_t, ALTI_FILTER_MAX_DEPTH>::SensorValues(m_alti_values.main_alti, m_alti_values.min_alti, m_alti_values.max_alti))

, m_config_values(OV_CONFIG_GROUP_ALTIMETER, "Altimeter")
, m_config_ref_alti(OV_CONFIG_VALUE_ALTI_REF_ALTI, "Reference altitude", 0, true)
, m_config_ref_temp(OV_CONFIG_VALUE_ALTI_REF_TEMP, "Reference temperature", 0, true)
, m_config_ref_pres(OV_CONFIG_VALUE_ALTI_REF_PRES, "Reference pressure", 0u, true)
, m_config_filter_depth(OV_CONFIG_VALUE_ALTI_FILTER_DEPTH, "Raw value filter depth", 5u, 1u, ALTI_FILTER_MAX_DEPTH, false)
, m_config_offset_alti1(OV_CONFIG_VALUE_ALTI_OFFSET_ALTI1, "Offset for altitude 1", 0, true)
, m_config_offset_alti2(OV_CONFIG_VALUE_ALTI_OFFSET_ALTI2, "Offset for altitude 2", 0, true)
, m_config_offset_alti3(OV_CONFIG_VALUE_ALTI_OFFSET_ALTI3, "Offset for altitude 3", 0, true)
, m_config_offset_alti4(OV_CONFIG_VALUE_ALTI_OFFSET_ALTI4, "Offset for altitude 4", 0, true)

, m_started(false)


, m_offset_alti_1(0)
, m_offset_alti_2(0)
, m_offset_alti_3(0)
, m_offset_alti_4(0)

, m_listeners()
{
    // Register configuration values
    m_config_values.registerConfigValue(m_config_ref_alti);
    m_config_values.registerConfigValue(m_config_ref_temp);
    m_config_values.registerConfigValue(m_config_ref_pres);
    m_config_values.registerConfigValue(m_config_filter_depth);
    m_config_values.registerConfigValue(m_config_offset_alti1);
    m_config_values.registerConfigValue(m_config_offset_alti2);
    m_config_values.registerConfigValue(m_config_offset_alti3);
    m_config_values.registerConfigValue(m_config_offset_alti4);
    m_config_manager.registerConfigValueGroup(m_config_values);
}


/** \brief Initialize the altimeter */
bool Altimeter::init()
{
    bool ret;

    // Load configuration values
    int32_t ref_alti = 0u;
    int16_t ref_temp = 0u;
    uint32_t ref_pres = 0u;
    ret = m_config_manager.getConfigValue<int32_t>(OV_CONFIG_GROUP_ALTIMETER, OV_CONFIG_VALUE_ALTI_REF_ALTI, ref_alti);
    ret = ret && m_config_manager.getConfigValue<int16_t>(OV_CONFIG_GROUP_ALTIMETER, OV_CONFIG_VALUE_ALTI_REF_TEMP, ref_temp);
    ret = ret && m_config_manager.getConfigValue<uint32_t>(OV_CONFIG_GROUP_ALTIMETER, OV_CONFIG_VALUE_ALTI_REF_PRES, ref_pres);
    ret = ret && m_config_manager.getConfigValue<int32_t>(OV_CONFIG_GROUP_ALTIMETER, OV_CONFIG_VALUE_ALTI_OFFSET_ALTI1, m_offset_alti_1);
    ret = ret && m_config_manager.getConfigValue<int32_t>(OV_CONFIG_GROUP_ALTIMETER, OV_CONFIG_VALUE_ALTI_OFFSET_ALTI2, m_offset_alti_2);
    ret = ret && m_config_manager.getConfigValue<int32_t>(OV_CONFIG_GROUP_ALTIMETER, OV_CONFIG_VALUE_ALTI_OFFSET_ALTI3, m_offset_alti_3);
    ret = ret && m_config_manager.getConfigValue<int32_t>(OV_CONFIG_GROUP_ALTIMETER, OV_CONFIG_VALUE_ALTI_OFFSET_ALTI4, m_offset_alti_4);
    if (ret)
    {
        // Initialize filter
        ret = m_alti_filter.init();
        if (ret)
        {
            // Configure the barometric altimeter
            if (!((ref_alti == 0) && (ref_temp == 0) && (ref_pres == 0u)))
            {
                m_barometric_altimeter.setReferences(ref_temp, ref_pres, ref_alti);
            }
        }
    }

    return ret;
}

/** \brief Start the altimeter */
bool Altimeter::start()
{
    bool ret = false;

    if (!m_started)
    {
        m_started = true;
        ret = true;
    }

    return ret;
}

/** \brief Compute altitude and get current sensor raw values */
bool Altimeter::compute(int32_t& raw_altitude, uint32_t& raw_pressure, int16_t& raw_temperature)
{
    bool ret;

    // Read sensor raw values
    ret = m_barometric_altimeter.readAltitude(raw_altitude);
    ret = ret && m_barometric_altimeter.readPressure(raw_pressure);
    ret = ret && m_barometric_altimeter.readTemperature(raw_temperature);
    if (ret)
    {
        // Compute filtered value
        m_alti_filter.compute(raw_altitude);

        // Update secondary altitudes
        m_alti_values.alti_1 = m_alti_values.main_alti + m_offset_alti_1;
        m_alti_values.alti_2 = m_alti_values.main_alti + m_offset_alti_2;
        m_alti_values.alti_3 = m_alti_values.main_alti + m_offset_alti_3;
        m_alti_values.alti_4 = m_alti_values.main_alti + m_offset_alti_4;

        // Notify current values
        if (m_started)
        {
            for (nano_stl::nano_stl_size_t i = 0; i < m_listeners.getCount(); i++)
            {
                m_listeners[i]->onAltimeterValues(m_alti_values);
            }
        }
    }

    return ret;
}

/** \brief Set a reference altitude (1 = 0.1m) */
bool Altimeter::setReferenceAltitude(const int32_t ref_altitude)
{
    const bool ret = m_barometric_altimeter.setReferenceAltitude(ref_altitude);
    if (ret)
    {
        // Reset min and max values
        m_alti_values.min_alti = ref_altitude;
        m_alti_values.max_alti = ref_altitude;
    }
    return ret;
}

/** \brief Register a listener for the altimeter values */
bool Altimeter::registerListener(IAltimeterListener& listener)
{
    const bool ret = m_listeners.pushBack(&listener);
    return ret;
}


}
