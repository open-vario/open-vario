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

#ifndef DATASERIALIZER_H
#define DATASERIALIZER_H

#include "nano-stl.h"
#include "IRtc.h"
#include "FlightData.h"

namespace open_vario
{

/** \brief Data serializer */
class DataSerializer
{
    public:

        /** \brief Constructor */
        DataSerializer(uint8_t* buffer, const size_t size);


        /** \brief Get the serialized buffer */
        const uint8_t* getBuffer() const { return m_serialized_buffer; }

        /** \brief Get the serialized data size */
        const size_t getBufferSize() const { return m_serialized_data_size; }


        /** \brief Clear buffer */
        void clear() { m_serialized_data_size = 0; }

        /** \brief Serialize 8 bits unsigned integer */
        bool serialize(const uint8_t val) { return serialize(&val, sizeof(val)); }

        /** \brief Serialize 16 bits unsigned integer */
        bool serialize(const uint16_t val) { return serialize(&val, sizeof(val)); }

        /** \brief Serialize 32 bits unsigned integer */
        bool serialize(const uint32_t val) { return serialize(&val, sizeof(val)); }

        /** \brief Serialize 64 bits unsigned integer */
        bool serialize(const uint64_t val) { return serialize(&val, sizeof(val)); }

        /** \brief Serialize 8 bits signed integer */
        bool serialize(const int8_t val) { return serialize(static_cast<uint8_t>(val)); }

        /** \brief Serialize 16 bits signed integer */
        bool serialize(const int16_t val) { return serialize(static_cast<uint16_t>(val)); }

        /** \brief Serialize 32 bits signed integer */
        bool serialize(const int32_t val) { return serialize(static_cast<uint32_t>(val)); }

        /** \brief Serialize 64 bits signed integer */
        bool serialize(const int64_t val) { return serialize(static_cast<uint64_t>(val)); }

        /** \brief Serialize single precision floating point value */
        bool serialize(const float val) { return serialize(&val, sizeof(val)); }

        /** \brief Serialize double precision floating point value */
        bool serialize(const double val) { return serialize(&val, sizeof(val)); }

        /** \brief Serialize a string */
        bool serialize(const char* s);

        /** \brief Serialize a buffer */
        bool serialize(const void* buffer, const size_t size);

        /** \brief Serialize a date and time structure */
        bool serialize(const IRtc::DateTime& date_time);

        /** \brief Serialize a flight data structure */
        bool serialize(const FlightData& flight_data);


    private:

        /** \brief Serialized buffer */
        uint8_t* m_serialized_buffer;

        /** \brief Serialized buffer size */
        const size_t m_serialized_buffer_size;

        /** \brief Serialized buffer size */
        size_t m_serialized_data_size;
};

}

#endif // DATASERIALIZER_H
