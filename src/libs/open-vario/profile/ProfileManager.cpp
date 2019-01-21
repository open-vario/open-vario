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

#include "ProfileManager.h"
#include "OpenVarioConfig.h"
#include "LogMacro.h"

namespace open_vario
{


/** \brief Constructor */
ProfileManager::ProfileManager(ConfigManager& config_manager)
: m_config_manager(config_manager)

, m_config_values(OV_CONFIG_GROUP_PROFILE, "Profile manager")
, m_config_pilot(OV_CONFIG_VALUE_PROFILE_PILOT, "Selected pilot", 0u, 0u, (MAX_PILOT_COUNT - 1u), false)
, m_config_glider(OV_CONFIG_VALUE_PROFILE_GLIDER, "Selected glider", 0u, 0u, (MAX_GLIDER_COUNT - 1u), false)
, m_config_pilot1(OV_CONFIG_VALUE_PROFILE_PILOT1_NAME, "Pilot 1 name", "Pilot 1", false)
, m_config_pilot2(OV_CONFIG_VALUE_PROFILE_PILOT2_NAME, "Pilot 2 name", "Pilot 2", false)
, m_config_pilot3(OV_CONFIG_VALUE_PROFILE_PILOT3_NAME, "Pilot 3 name", "Pilot 3", false)
, m_config_glider1(OV_CONFIG_VALUE_PROFILE_GLIDER1_NAME, "Glider 1 name", "Glider 1", false)
, m_config_glider2(OV_CONFIG_VALUE_PROFILE_GLIDER2_NAME, "Glider 2 name", "Glider 2", false)
, m_config_glider3(OV_CONFIG_VALUE_PROFILE_GLIDER3_NAME, "Glider 3 name", "Glider 3", false)
, m_config_glider4(OV_CONFIG_VALUE_PROFILE_GLIDER4_NAME, "Glider 4 name", "Glider 4", false)
, m_config_glider5(OV_CONFIG_VALUE_PROFILE_GLIDER5_NAME, "Glider 5 name", "Glider 5", false)

, m_pilot(0u)
, m_glider(0u)
, m_pilot_name()
, m_glider_name()
{
    // Register configuration values
    m_config_values.registerConfigValue(m_config_pilot);
    m_config_values.registerConfigValue(m_config_glider);
    m_config_values.registerConfigValue(m_config_pilot1);
    m_config_values.registerConfigValue(m_config_pilot2);
    m_config_values.registerConfigValue(m_config_pilot3);
    m_config_values.registerConfigValue(m_config_glider1);
    m_config_values.registerConfigValue(m_config_glider2);
    m_config_values.registerConfigValue(m_config_glider3);
    m_config_values.registerConfigValue(m_config_glider4);
    m_config_values.registerConfigValue(m_config_glider5);
    m_config_manager.registerConfigValueGroup(m_config_values);
}

/** \brief Initialize the profile manager */
bool ProfileManager::init()
{
    bool ret;

    LOG_INFO("Initializing profile manager...");

    // Load configuration values
    ret = m_config_manager.getConfigValue<uint8_t>(OV_CONFIG_GROUP_PROFILE, OV_CONFIG_VALUE_PROFILE_PILOT, m_pilot);
    ret = ret && m_config_manager.getConfigValue<uint8_t>(OV_CONFIG_GROUP_PROFILE, OV_CONFIG_VALUE_PROFILE_GLIDER, m_glider);
    if (ret)
    {
        // Update pilot's and glider's names
        ret = updatePilotName();
        ret = ret && updateGliderName();
        if (ret)
        {
            // Register to configuration changes
            ret = m_config_manager.registerConfigValueListener(OV_CONFIG_GROUP_PROFILE, OV_CONFIG_VALUE_PROFILE_PILOT, *this);
            ret = ret && m_config_manager.registerConfigValueListener(OV_CONFIG_GROUP_PROFILE, OV_CONFIG_VALUE_PROFILE_GLIDER, *this);            
        }
    }

    return ret;
}

/** \brief Select the pilot */
bool ProfileManager::setPilot(const uint8_t pilot)
{
    const bool ret = m_config_manager.setConfigValue<uint8_t>(OV_CONFIG_GROUP_PROFILE, OV_CONFIG_VALUE_PROFILE_PILOT, pilot);
    return ret;
}

/** \brief Select the glider */
bool ProfileManager::setGlider(const uint8_t glider)
{
    const bool ret = m_config_manager.setConfigValue<uint8_t>(OV_CONFIG_GROUP_PROFILE, OV_CONFIG_VALUE_PROFILE_GLIDER, glider);
    return ret;
}

/** \brief Called when a configuration value has been modified */
void ProfileManager::onConfigValueChange(const IConfigValue& config_value)
{
    switch(config_value.id())
    {
        case OV_CONFIG_VALUE_PROFILE_PILOT:
        {
            // Pilot changed
            m_pilot = config_value.value<uint8_t>();
            updatePilotName();
            break;
        }

        case OV_CONFIG_VALUE_PROFILE_GLIDER:
        {
            // Glider changed
            m_glider = config_value.value<uint8_t>();
            updateGliderName();
            break;
        }

        default:
        {
            // Should not happen => ignore
            break;
        }
    }
}

/** \brief Update the pilot's name */
bool ProfileManager::updatePilotName()
{
    const bool ret = m_config_manager.getConfigValue(OV_CONFIG_GROUP_PROFILE, OV_CONFIG_VALUE_PROFILE_PILOT1_NAME + m_pilot, m_pilot_name);
    LOG_INFO("Selected pilot : %s", m_pilot_name);
    return ret;
}

/** \brief Update the glider's name */
bool ProfileManager::updateGliderName()
{
    const bool ret = m_config_manager.getConfigValue(OV_CONFIG_GROUP_PROFILE, OV_CONFIG_VALUE_PROFILE_GLIDER1_NAME + m_glider, m_glider_name);
    LOG_INFO("Selected glider : %s", m_glider_name);
    return ret;
}

}
