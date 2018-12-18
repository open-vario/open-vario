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
        uint16_t getCount() { return static_cast<uint16_t>(m_config_value_groups.getCount()); }

        /** \brief Look for a configuration value group */
        bool searchConfigValueGroup(const char* const name, uint16_t& id);

        /** \brief Get a configuration value group */
        bool getConfigValueGroup(const uint16_t id, IConfigValueGroup*& config_value_group);


    private:

        /** \brief Current configuration version */
        const uint16_t m_config_version;

        /** \brief Configuration area accessor */
        IConfigAreaAccessor& m_config_area_accessor;

        /** \brief Groups of configuration values */
        nano_stl::StaticArray<IConfigValueGroup*, 100u> m_config_value_groups;
};

}

#endif // CONFIGMANAGER_H
