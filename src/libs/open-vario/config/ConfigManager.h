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

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "ConfigValueGroup.h"
#include "StaticArray.h"
#include "IConfigAreaAccessor.h"

namespace open_vario
{


/** \brief Configuration manager */
class ConfigManager
{
    public:


        /** \brief Configuration value informations */
        struct ConfigValueInfos
        {
            /** \brief Name */
            const char* name;
            /** \brief Type */
            const char* type;
            /** \brief Size in bytes */
            uint32_t size;
            /** \brief Indicate if the value has a min and max value */
            bool has_min_max;
            /** \brief Indicate if the value will be taken into account only after a reset */
            bool is_reset_only;
            /** \brief Buffer representing the min value */
            const uint8_t* min_value;
            /** \brief Buffer representing the max value */
            const uint8_t* max_value;
        };


        /** \brief Constructor */
        ConfigManager(const uint16_t config_version, IConfigAreaAccessor& config_area_accessor);


        /** \brief Register a configuration value group */
        bool registerConfigValueGroup(IConfigValueGroup& config_value_group);



        /** \brief Initialize the configuration manager */
        bool init();

        /** \brief Store configuration to configuration area */
        bool store();

        /** \brief Reset to default configuration values */
        void resetToDefault();

        

        /** \brief Get the number of configuration value groups */
        uint16_t getConfigValueGroupCount() { return static_cast<uint16_t>(m_config_value_groups.getCount()); }

        /** \brief Get a configuration value group */
        bool getConfigValueGroup(const uint16_t id, IConfigValueGroup*& config_value_group);

        /** \brief Get informations on a configuration value */
        bool getConfigValueInfos(const uint16_t config_value_group_id, const uint16_t config_value_id, ConfigValueInfos& config_value_infos);


        /** \brief Get a configuration value */
        template <typename T>
        bool getConfigValue(const uint16_t config_value_group_id, const uint16_t config_value_id, T& value)
        {
            IConfigValue* config_value = nullptr;
            IConfigValueGroup* config_value_group = nullptr;
            const bool ret = getConfigValueAndGroup(config_value_group_id, config_value_id, config_value_group, config_value);
            if (ret)
            {
                config_value_group->lock();
                config_value->get(reinterpret_cast<uint8_t*>(&value));
                config_value_group->unlock();
            }

            return ret;
        }

        /** \brief Get a configuration value */
        bool getConfigValue(const uint16_t config_value_group_id, const uint16_t config_value_id, char* value);
        
        /** \brief Set a configuration value */
        template <typename T>
        bool setConfigValue(const uint16_t config_value_group_id, const uint16_t config_value_id, const T& value)
        {
            IConfigValue* config_value = nullptr;
            IConfigValueGroup* config_value_group = nullptr;
            const bool ret = getConfigValueAndGroup(config_value_group_id, config_value_id, config_value_group, config_value);
            if (ret)
            {
                config_value_group->lock();
                config_value->set(reinterpret_cast<const uint8_t*>(&value));
                config_value_group->unlock();
            }

            return ret;
        }

        /** \brief Set a configuration value */
        bool setConfigValue(const uint16_t config_value_group_id, const uint16_t config_value_id, const char* value);

        /** \brief Register a listener to a configuration value */
        bool registerConfigValueListener(const uint16_t config_value_group_id, const uint16_t config_value_id, IConfigValueListener& listener);


    private:

        /** \brief Current configuration version */
        const uint16_t m_config_version;

        /** \brief Configuration area accessor */
        IConfigAreaAccessor& m_config_area_accessor;

        /** \brief Groups of configuration values */
        nano_stl::StaticArray<IConfigValueGroup*, 15u> m_config_value_groups;


        /** \brief Get a configuration value and its group */
        bool getConfigValueAndGroup(const uint16_t config_value_group_id, const uint16_t config_value_id, IConfigValueGroup*& config_value_group, IConfigValue*& config_value);
};


}

#endif // CONFIGMANAGER_H
