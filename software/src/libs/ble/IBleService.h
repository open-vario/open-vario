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

#ifndef IBLESERVICE_H
#define IBLESERVICE_H

#include "IBleAttribute.h"

namespace nano_stl
{

template <typename ItemType>
class IArray;

};

namespace open_vario
{

class IBleCharacteristic;

/** \brief Interface for all Bluetooth Low Energy services implementations */
class IBleService : public IBleAttribute
{
    public:

        /** \brief Get the service's included services */
        virtual const nano_stl::IArray<IBleService*>& services() const = 0;

        /** \brief Get the service's characteristics */
        virtual const nano_stl::IArray<IBleCharacteristic*>& characteristics() const = 0;

};

}

#endif // IBLESERVICE_H
