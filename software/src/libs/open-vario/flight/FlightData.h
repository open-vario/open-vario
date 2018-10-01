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
    /** \brief Altitude : 1 = 1m */
    uint16_t altitude;
    /** \brief Vario : 1 = 0.1 m/s */
    uint16_t vario;
    /** \brief Acceleration : 1 = 0.1 g */
    uint8_t accelleration;
    /** \brief Speed : 1 = 0.1 m/s */
    uint32_t speed;
    /** \brief Latitude : 1 = 1° */
    double latitude;
    /** \brief Longitude : 1 = 1° */
    double longitude;
    /** \brief Temperature : 1 = 0.1 °C */
    int16_t temperature;
};


}

#endif // FLIGHTDATA_H
