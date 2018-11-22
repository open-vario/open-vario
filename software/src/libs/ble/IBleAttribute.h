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

#ifndef IBLEATTRIBUTE_H
#define IBLEATTRIBUTE_H

#include <cstdint>


namespace open_vario
{

class IBleUuid;

/** \brief Interface for all Bluetooth Low Energy GATT attribute implementations */
class IBleAttribute
{
    public:

        /** \brief Get the attribute's name */
        virtual const char* name() const = 0;

        /** \brief Get the attribute's UUID */
        virtual const IBleUuid& uuid() const = 0;

        /** \brief Set the stack handle */
        virtual void setHandle(const uint16_t handle) = 0;

        /** \brief Get the stack handle */
        virtual uint16_t handle() const = 0;

};

}

#endif // IBLEATTRIBUTE_H
