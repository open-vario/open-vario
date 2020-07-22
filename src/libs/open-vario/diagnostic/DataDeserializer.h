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

#ifndef DATADESERIALIZER_H
#define DATADESERIALIZER_H

#include "nano-stl.h"
#include "IRtc.h"

namespace open_vario
{

/** \brief Data deserializer */
class DataDeserializer
{
    public:

        /** \brief Constructor */
        DataDeserializer(const uint8_t* buffer, const size_t size);


        /** \brief Deserialize 8 bits unsigned integer */
        bool deserialize(uint8_t& val) { return deserialize(&val, sizeof(val)); }

        /** \brief Deserialize 16 bits unsigned integer */
        bool deserialize(uint16_t& val) { return deserialize(&val, sizeof(val)); }

        /** \brief Deserialize 32 bits unsigned integer */
        bool deserialize(uint32_t& val) { return deserialize(&val, sizeof(val)); }

        /** \brief Deserialize 64 bits unsigned integer */
        bool deserialize(uint64_t& val) { return deserialize(&val, sizeof(val)); }

        /** \brief Deserialize 8 bits signed integer */
        bool deserialize(int8_t& val) { return deserialize(reinterpret_cast<uint8_t&>(val)); }

        /** \brief Deserialize 16 bits signed integer */
        bool deserialize(int16_t& val) { return deserialize(reinterpret_cast<uint16_t&>(val)); }

        /** \brief Deserialize 32 bits signed integer */
        bool deserialize(int32_t& val) { return deserialize(reinterpret_cast<uint32_t&>(val)); }

        /** \brief Deserialize 64 bits signed integer */
        bool deserialize(int64_t& val) { return deserialize(reinterpret_cast<uint64_t&>(val)); }

        /** \brief Deserialize a string */
        bool deserialize(char* s);

        /** \brief Deserialize a date and time */
        bool deserialize(IRtc::DateTime& date_time);

        /** \brief Deserialize a buffer */
        bool deserialize(void* buffer, const size_t size);


    private:

        /** \brief Serialized buffer */
        const uint8_t* m_serialized_buffer;

        /** \brief Serialized data size */
        size_t m_serialized_data_size;
};

}

#endif // DATADESERIALIZER_H
