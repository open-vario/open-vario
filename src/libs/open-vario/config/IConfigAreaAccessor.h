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

#ifndef ICONFIGAREAACCESSOR_H
#define ICONFIGAREAACCESSOR_H

#include <cstdint>
#include "IArray.h"
#include "IConfigValueGroup.h"

namespace open_vario
{


/** \brief Interface for all configuration area accessors implementations */
class IConfigAreaAccessor
{
    public:

        /** \brief Load the configuration from the configuration area */
        virtual bool load(const uint16_t config_version, const nano_stl::IArray<IConfigValueGroup*>& m_config_value_groups, bool& compatibility_mode) = 0;

        /** \brief Store the configuration to the configuration area */
        virtual bool store(const uint16_t config_version, const nano_stl::IArray<IConfigValueGroup*>& m_config_value_groups) = 0;

};

}

#endif // ICONFIGAREAACCESSOR_H
