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

#ifndef FLIGHTDATA_H
#define FLIGHTDATA_H

#include <cstdint>

namespace open_vario
{

/** \brief Flight data */
struct FlightData
{
    /** \brief Altitude (1 = 0.1m) */
    int32_t altitude;
    /** \brief Pressure (1 = 0.01mbar) */
    uint32_t pressure;
    /** \brief Temperature (1 = 0.1°C) */
    int16_t temperature;
    /** \brief Vario (1 = 0.1m/s) */
    int16_t vario;
    /** \brief Acceleration (1 = 0.1g) */
    uint16_t acceleration;
    /** \brief Latitude (1 = 1°) */
    double latitude;
    /** \brief Longitude (1 = 1°) */
    double longitude;
    /** \brief Speed (1 = 0.1m/s) */
    uint16_t speed;
    /** \brief Track angle (1 = 0.1°) */
    uint16_t track_angle;

    /** \brief Comparison operator */
    bool operator == (const FlightData& fligh_data) const
    {
        return ((altitude == fligh_data.altitude) &&
                (pressure == fligh_data.pressure) &&
                (temperature == fligh_data.temperature) &&
                (vario == fligh_data.vario) &&
                (acceleration == fligh_data.acceleration) &&
                (latitude == fligh_data.latitude) &&
                (longitude == fligh_data.longitude) &&
                (speed == fligh_data.speed) &&
                (track_angle == fligh_data.track_angle));
    }

    /** \brief Comparison operator */
    bool operator != (const FlightData& fligh_data) const
    {
        return !(operator == (fligh_data));
    }
};

}

#endif // FLIGHTDATA_H
