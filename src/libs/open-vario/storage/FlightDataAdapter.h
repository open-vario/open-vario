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

#ifndef FLIGHTDATAADAPTER_H
#define FLIGHTDATAADAPTER_H

#include "FlightData.h"
#include "nano-stl-conf.h"

namespace open_vario
{

/** \brief Flight data adapter */
class FlightDataAdapter
{
    public:

        /** \brief Default constructor */
        FlightDataAdapter()
        : m_pressure_recording_enabled(false)
        , m_temperature_recording_enabled(false)
        , m_acceleration_recording_enabled(false)
        , m_gnss_recording_enabled(false)
        , m_size(computeSize())
        {}

        /** \brief Constructor */
        FlightDataAdapter(const bool pressure_recording_enabled,
                          const bool temperature_recording_enabled,
                          const bool acceleration_recording_enabled,
                          const bool gnss_recording_enabled)
        : m_pressure_recording_enabled(pressure_recording_enabled)
        , m_temperature_recording_enabled(temperature_recording_enabled)
        , m_acceleration_recording_enabled(acceleration_recording_enabled)
        , m_gnss_recording_enabled(gnss_recording_enabled)
        , m_size(computeSize())
        {}

        /** \brief Configure the adapter */
        void configure(const bool pressure_recording_enabled,
                       const bool temperature_recording_enabled,
                       const bool acceleration_recording_enabled,
                       const bool gnss_recording_enabled)
        {
            m_pressure_recording_enabled = pressure_recording_enabled;
            m_temperature_recording_enabled = temperature_recording_enabled;
            m_acceleration_recording_enabled = acceleration_recording_enabled;
            m_gnss_recording_enabled = gnss_recording_enabled;
            m_size = computeSize();
        }

        /** \brief Get the size of the serialized flight data structure */
        uint8_t size() const { return m_size; }        
        
        /** \brief Serialize a flight data structure to an array of bytes */
        uint8_t serialize(const FlightData& flight_data, uint8_t serialized_data[])
        {
            uint8_t index = 0u;
            
            NANO_STL_MEMCPY(&serialized_data[index], &flight_data.altitude, sizeof(flight_data.altitude));
            index += sizeof(flight_data.altitude);

            if (m_pressure_recording_enabled)
            {
                NANO_STL_MEMCPY(&serialized_data[index], &flight_data.pressure, sizeof(flight_data.pressure));
                index += sizeof(flight_data.pressure);
            }

            if (m_temperature_recording_enabled)
            {
                NANO_STL_MEMCPY(&serialized_data[index], &flight_data.temperature, sizeof(flight_data.temperature));
                index += sizeof(flight_data.temperature);
            }

            NANO_STL_MEMCPY(&serialized_data[index], &flight_data.vario, sizeof(flight_data.vario));
            index += sizeof(flight_data.vario);

            if (m_acceleration_recording_enabled)
            {
                NANO_STL_MEMCPY(&serialized_data[index], &flight_data.acceleration, sizeof(flight_data.acceleration));
                index += sizeof(flight_data.acceleration);
            }

            if (m_gnss_recording_enabled)
            {
                NANO_STL_MEMCPY(&serialized_data[index], &flight_data.latitude, sizeof(flight_data.latitude));
                index += sizeof(flight_data.latitude);

                NANO_STL_MEMCPY(&serialized_data[index], &flight_data.longitude, sizeof(flight_data.longitude));
                index += sizeof(flight_data.longitude);

                NANO_STL_MEMCPY(&serialized_data[index], &flight_data.speed, sizeof(flight_data.speed));
                index += sizeof(flight_data.speed);

                NANO_STL_MEMCPY(&serialized_data[index], &flight_data.track_angle, sizeof(flight_data.track_angle));
                index += sizeof(flight_data.track_angle);
            }

            return index;
        }

        /** \brief Deserialize a flight data structure from an array of bytes */
        void deserialize(FlightData& flight_data, const uint8_t serialized_data[])
        {
            uint8_t index = 0u;
            
            NANO_STL_MEMCPY(&flight_data.altitude, &serialized_data[index], sizeof(flight_data.altitude));
            index += sizeof(flight_data.altitude);

            if (m_pressure_recording_enabled)
            {
                NANO_STL_MEMCPY(&flight_data.pressure, &serialized_data[index], sizeof(flight_data.pressure));
                index += sizeof(flight_data.pressure);
            }

            if (m_temperature_recording_enabled)
            {
                NANO_STL_MEMCPY(&flight_data.temperature, &serialized_data[index], sizeof(flight_data.temperature));
                index += sizeof(flight_data.temperature);
            }

            NANO_STL_MEMCPY(&flight_data.vario, &serialized_data[index], sizeof(flight_data.vario));
            index += sizeof(flight_data.vario);

            if (m_acceleration_recording_enabled)
            {
                NANO_STL_MEMCPY(&flight_data.acceleration, &serialized_data[index], sizeof(flight_data.acceleration));
                index += sizeof(flight_data.acceleration);
            }

            if (m_gnss_recording_enabled)
            {
                NANO_STL_MEMCPY(&flight_data.latitude, &serialized_data[index], sizeof(flight_data.latitude));
                index += sizeof(flight_data.latitude);

                NANO_STL_MEMCPY(&flight_data.longitude, &serialized_data[index], sizeof(flight_data.longitude));
                index += sizeof(flight_data.longitude);

                NANO_STL_MEMCPY(&flight_data.speed, &serialized_data[index], sizeof(flight_data.speed));
                index += sizeof(flight_data.speed);

                NANO_STL_MEMCPY(&flight_data.track_angle, &serialized_data[index], sizeof(flight_data.track_angle));
                index += sizeof(flight_data.track_angle);
            }
        }


    private:

        /** \brief Indicate if the pressure recording is enabled */
        bool m_pressure_recording_enabled;

        /** \brief Indicate if the temperature recording is enabled */
        bool m_temperature_recording_enabled;

        /** \brief Indicate if the acceleration recording is enabled */
        bool m_acceleration_recording_enabled;

        /** \brief Indicate if the GNSS recording is enabled */
        bool m_gnss_recording_enabled;

        /** \brief Flight data serialized size */
        uint8_t m_size;


        /** \brief Compute the size of the serialized flight data structure */
        uint8_t computeSize() const
        {
            uint8_t data_size = 0;

            data_size += sizeof(FlightData::altitude);

            if (m_pressure_recording_enabled)
            {
                data_size += sizeof(FlightData::pressure);
            }

            if (m_temperature_recording_enabled)
            {
                data_size += sizeof(FlightData::temperature);
            }

            data_size += sizeof(FlightData::vario);

            if (m_acceleration_recording_enabled)
            {
                data_size += sizeof(FlightData::acceleration);
            }

            if (m_gnss_recording_enabled)
            {
                data_size += sizeof(FlightData::latitude);
                data_size += sizeof(FlightData::longitude);
                data_size += sizeof(FlightData::speed);
                data_size += sizeof(FlightData::track_angle);
            }

            return data_size;
        }
};

}

#endif // FLIGHTDATAADAPTER_H
