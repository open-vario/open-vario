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

#include "DataDeserializer.h"


namespace open_vario
{


/** \brief Constructor */
DataDeserializer::DataDeserializer(const uint8_t* buffer, const size_t size)
: m_serialized_buffer(buffer)
, m_serialized_data_size(size)
{
}

/** \brief Deserialize a string */
bool DataDeserializer::deserialize(char* s)
{
    return deserialize(s, NANO_STL_STRNLEN(reinterpret_cast<const char*>(&m_serialized_buffer[m_serialized_data_size]), m_serialized_data_size) + 1u);
}

/** \brief Deserialize a buffer */
bool DataDeserializer::deserialize(void* buffer, const size_t size)
{
    bool ret = false;

    if (m_serialized_data_size >= size)
    {
        NANO_STL_MEMCPY(buffer, &m_serialized_buffer[m_serialized_data_size], size);
        m_serialized_data_size -= size;
        ret = true;
    } 

    return ret;
}

}
