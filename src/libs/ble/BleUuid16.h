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

#ifndef BLEUUID16_H
#define BLEUUID16_H

#include "IBleUuid.h"
#include "StaticArray.h"

#include <initializer_list>

namespace open_vario
{


/** \brief Bluetooth Low Energy 16 bits UUID */
class BleUuid16 : public IBleUuid
{
    public:

        /** \brief Constructor */
        BleUuid16(const std::initializer_list<uint8_t>& l)
        : m_uuid()
        {
            if (l.size() == 2u)
            {
                const uint8_t* uuid = l.begin();
                m_uuid[0u] = uuid[0u];
                m_uuid[1u] = uuid[1u];
            }
        }

        /** \brief Constructor */
        BleUuid16(const uint16_t uuid)
        : m_uuid()
        {
            m_uuid[0u] = static_cast<uint8_t>(uuid & 0xFFu);
            m_uuid[1u] = static_cast<uint8_t>((uuid >> 8u) & 0xFFu);
        }

        /** \brief Get the UUID's as an array of bytes */
        virtual const nano_stl::IArray<uint8_t>& value() const { return m_uuid; }

    private:

        /** \brief Internal array representing the UUID */
        nano_stl::StaticArray<uint8_t, 2u> m_uuid;

};

}

#endif // BLEUUID16_H
