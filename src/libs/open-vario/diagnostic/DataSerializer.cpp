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

#include "DataSerializer.h"


namespace open_vario
{


/** \brief Constructor */
DataSerializer::DataSerializer(uint8_t* buffer, const size_t size)
: m_serialized_buffer(buffer)
, m_serialized_buffer_size(size)
, m_serialized_data_size(0)
{
}

/** \brief Serialize a string */
bool DataSerializer::serialize(const char* s)
{
    return serialize(s, NANO_STL_STRNLEN(s, m_serialized_buffer_size) + 1u);
}

/** \brief Serialize a buffer */
bool DataSerializer::serialize(const void* buffer, const size_t size)
{
    bool ret = false;

    if ((m_serialized_data_size + size) <= m_serialized_buffer_size)
    {
        NANO_STL_MEMCPY(&m_serialized_buffer[m_serialized_data_size], buffer, size);
        m_serialized_data_size += size;
        ret = true;
    } 

    return ret;
}

/** \brief Serialize a date and time structure */
bool DataSerializer::serialize(const IRtc::DateTime& date_time)
{
    bool ret = false;

    if ((m_serialized_data_size + 8u) <= m_serialized_buffer_size)
    {
        serialize(date_time.year);
        serialize(date_time.month);
        serialize(date_time.day);
        serialize(date_time.hour);
        serialize(date_time.minute);
        serialize(date_time.second);
        serialize(date_time.millis);
        ret = true;
    }

    return ret;
}

/** \brief Serialize a flight data structure */
bool DataSerializer::serialize(const FlightData& flight_data)
{
    bool ret = false;

    if ((m_serialized_data_size + 34u) <= m_serialized_buffer_size)
    {
        serialize(flight_data.altitude);
        serialize(flight_data.latitude);
        serialize(flight_data.longitude);
        serialize(flight_data.vario);
        serialize(flight_data.speed);
        serialize(flight_data.track_angle);
        serialize(flight_data.acceleration);
        serialize(flight_data.temperature);
        serialize(flight_data.pressure);
        ret = true;
    }

    return ret;
}

}
