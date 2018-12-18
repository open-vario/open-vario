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

#include "ConfigManager.h"
#include "LogMacro.h"
#include "ForEach.h"
#include "IConfigValue.h"

namespace open_vario
{


/** \brief Constructor */
ConfigManager::ConfigManager(const uint16_t config_version, IConfigAreaAccessor& config_area_accessor)
: m_config_version(config_version)
, m_config_area_accessor(config_area_accessor)
{}

/** \brief Register a configuration value group */
bool ConfigManager::registerConfigValueGroup(IConfigValueGroup& config_value_group)
{
    bool ret = false;

    if ((config_value_group.id() < m_config_value_groups.getCount()) &&
        (m_config_value_groups[config_value_group.id()] == NULL))
    {
        m_config_value_groups[config_value_group.id()] = &config_value_group;
        ret = true;
    }

    return ret;
}

/** \brief Initialize the configuration manager */
bool ConfigManager::init()
{
    bool ret;
    bool compatibility_mode = false;

    // Load configuration values
    LOG_INFO("Loading configuration...");
    ret = m_config_area_accessor.load(m_config_version, m_config_value_groups, compatibility_mode);
    if (ret)
    {
        // If compatibility mode, save data into configuration area to update configuration area version
        if (compatibility_mode)
        {
            LOG_INFO("Configuration loaded from different configuration version");
            ret = store();
        } 
    }
    else
    {
        LOG_ERROR("Failed to load configuration...");
        resetToDefault();
        ret = store();
    }

    return ret;
}

/** \brief Store configuration to configuration area */
bool ConfigManager::store()
{
    LOG_INFO("Saving configuration...");
    
    const bool ret = m_config_area_accessor.store(m_config_version, m_config_value_groups);
    if (!ret)
    {
        LOG_ERROR("Failed to store configuration");
    }

    return ret;
}

/** \brief Reset to default configuration values */
void ConfigManager::resetToDefault()
{
    LOG_INFO("Resetting configuration to default values...");
    
    foreach(config_value_group, m_config_value_groups, IConfigValueGroup*)
    {
        if (config_value_group != NULL)
        {
            for (uint32_t i = 0; i < config_value_group->getCount(); i++)
            {
                IConfigValue* config_value;
                if (config_value_group->getConfigValue(i, config_value))
                {
                    config_value->reset();
                }
            }
        }
    }
}

/** \brief Look for a configuration value group */
bool ConfigManager::searchConfigValueGroup(const char* const name, uint16_t& id)
{
    bool ret = false;

    for (id = 0; (id < m_config_value_groups.getCount()) && !ret; id++)
    {
        ret = (strcmp(name, m_config_value_groups[id]->name()) == 0);
    }

    return ret;
}

/** \brief Get a configuration value group */
bool ConfigManager::getConfigValueGroup(const uint16_t id, IConfigValueGroup*& config_value_group)
{
    bool ret = false;

    if (id < m_config_value_groups.getCount())
    {
        config_value_group = m_config_value_groups[id];
    }

    return ret;
}


}
