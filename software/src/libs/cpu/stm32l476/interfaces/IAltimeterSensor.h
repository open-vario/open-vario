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

#ifndef IALTIMETERSENSOR_H
#define IALTIMETERSENSOR_H

#include <cstdint>

namespace open_vario
{



/** \brief Interface for all altimeter sensors implementations */
class IAltimeterSensor
{
    public:

        /** \brief Configure the altimeter sensor */
        virtual bool configure() = 0;

        /** \brief Read the altitude (1 = 0.1m) */
        virtual bool readAltitude(uint32_t& altitude) = 0;

        /** \brief Set a reference altitude (1 = 0.1m) */
        virtual bool setReferenceAltitude(const uint32_t ref_altitude) = 0;
};

}

#endif // IALTIMETERSENSOR_H
