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

#ifndef BLEUUID128_H
#define BLEUUID128_H

#include "IBleUuid.h"
#include "StaticArray.h"

#include <cstring>
#include <initializer_list>

namespace open_vario
{


/** \brief Bluetooth Low Energy 128 bits UUID */
class BleUuid128 : public IBleUuid
{
    public:


        /** \brief Constructor */
        BleUuid128(const std::initializer_list<uint8_t>& l)
        : m_uuid()
        {
            if (l.size() == m_uuid.getCount())
            {
                NANO_STL_MEMCPY(&m_uuid[0u], l.begin(), m_uuid.getCount());
            }
        }

        /** \brief Constructor */
        BleUuid128(const uint8_t uuid[])
        : m_uuid()
        {
            NANO_STL_MEMCPY(&m_uuid[0u], uuid, m_uuid.getCount());
        }

        /** \brief Get the UUID's as an array of bytes */
        virtual const nano_stl::IArray<uint8_t>& value() const { return m_uuid; }

    private:

        /** \brief Internal array representing the UUID */
        nano_stl::StaticArray<uint8_t, 16u> m_uuid;

};

}

#endif // BLEUUID128_H
