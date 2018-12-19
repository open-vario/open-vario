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

#ifndef ICONFIGVALUEGROUP_H
#define ICONFIGVALUEGROUP_H

#include <cstdint>

namespace open_vario
{

class IConfigValue;


/** \brief Interface for all group of configuration values implementations */
class IConfigValueGroup
{
    public:

        /** \brief Get the group id */
        virtual uint16_t id() = 0;

        /** \brief Get the group name */
        virtual const char* name() const = 0;

        /** \brief Get the number of configuration values in the group */
        virtual uint16_t getCount() = 0;

        /** \brief Lock the group */
        virtual void lock() = 0;

        /** \brief Unlock the group */
        virtual void unlock() = 0;

        /** \brief Register a configuration value */
        virtual bool registerConfigValue(IConfigValue& config_value) = 0;

        /** \brief Get a configuration value */
        virtual bool getConfigValue(const uint16_t index, IConfigValue*& config_value) = 0;

};

}

#endif // ICONFIGVALUEGROUP_H
