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
        BleUuid128(const char uuid[])
        : m_uuid()
        {
            if (NANO_STL_STRNLEN(uuid, 37u) == 36u)
            {
                m_uuid[15u] = NANO_STL_LIBC_Antoi(&uuid[0u], 16, 2u);
                m_uuid[14u] = NANO_STL_LIBC_Antoi(&uuid[2u], 16, 2u);
                m_uuid[13u] = NANO_STL_LIBC_Antoi(&uuid[4u], 16, 2u);
                m_uuid[12u] = NANO_STL_LIBC_Antoi(&uuid[6u], 16, 2u);
                m_uuid[11u] = NANO_STL_LIBC_Antoi(&uuid[9u], 16, 2u);
                m_uuid[10u] = NANO_STL_LIBC_Antoi(&uuid[11u], 16, 2u);
                m_uuid[9u] = NANO_STL_LIBC_Antoi(&uuid[14u], 16, 2u);
                m_uuid[8u] = NANO_STL_LIBC_Antoi(&uuid[16u], 16, 2u);
                m_uuid[7u] = NANO_STL_LIBC_Antoi(&uuid[19u], 16, 2u);
                m_uuid[6u] = NANO_STL_LIBC_Antoi(&uuid[21u], 16, 2u);
                m_uuid[5u] = NANO_STL_LIBC_Antoi(&uuid[24u], 16, 2u);
                m_uuid[4u] = NANO_STL_LIBC_Antoi(&uuid[26u], 16, 2u);
                m_uuid[3u] = NANO_STL_LIBC_Antoi(&uuid[28u], 16, 2u);
                m_uuid[2u] = NANO_STL_LIBC_Antoi(&uuid[30u], 16, 2u);
                m_uuid[1u] = NANO_STL_LIBC_Antoi(&uuid[32u], 16, 2u);
                m_uuid[0u] = NANO_STL_LIBC_Antoi(&uuid[34u], 16, 2u);
            }
        }

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
