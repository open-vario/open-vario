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

#ifndef CONFIGVALUEGROUP_H
#define CONFIGVALUEGROUP_H

#include "IConfigValueGroup.h"
#include "Mutex.h"
#include "StaticVector.h"

#include <cstring>

namespace open_vario
{


/** \brief Group of configuration values */
template <uint32_t MAX_CONFIG_VALUE_COUNT>
class ConfigValueGroup : public IConfigValueGroup
{
    public:

        /** \brief Constructor */
        ConfigValueGroup(const char* const name)
        : m_id(id)
        , m_name(name)
        , m_mutex()
        , m_config_values()
        {}


        /** \brief Get the group id */
        virtual uint16_t id() { return m_id; }

        /** \brief Get the group name */
        virtual const char* name() const { return m_name; }

        /** \brief Get the number of configuration values in the group */
        virtual uint16_t getCount() { return static_cast<uint16_t>(m_config_values.getCount()); }

        /** \brief Lock the group */
        virtual void lock() { m_mutex.lock(); }

        /** \brief Unlock the group */
        virtual void unlock() { m_mutex.unlock(); }

        /** \brief Look for a configuration value in the group */
        virtual bool searchConfigValue(const char* const name, uint16_t& index)
        {
            bool ret = false;

            for (index = 0; (index < m_config_values.getCount()) && !ret; index++)
            {
                ret = (strcmp(name, m_config_values[index]->name()) == 0);
            }

            return ret;
        }

        /** \brief Get a configuration value */
        virtual bool getConfigValue(const uint16_t index, IConfigValue*& config_value)
        {
            bool ret = false;

            if (index < m_config_values.getCount())
            {
                config_value = m_config_values[index];
            }

            return ret;
        }

    
    private:

        /** \brief Id */
        const uint16_t m_id;

        /** \brief Name */
        const char* const m_name;

        /** \brief Mutex */
        Mutex m_mutex;

        /** \brief Configuration values */
        nano_stl::StaticVector<IConfigValue*, MAX_CONFIG_VALUE_COUNT> m_config_values;
};

}

#endif // CONFIGVALUEGROUP_H
