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

#ifndef IBAROMETRICALTIMETER_H
#define IBAROMETRICALTIMETER_H

#include "IAltimeterSensor.h"
#include "IBarometricSensor.h"

namespace open_vario
{



/** \brief Interface for all barometric altimeter sensor implementations */
class IBarometricAltimeter : public IBarometricSensor, public IAltimeterSensor
{
    public:

        /** \brief Set the references for the altitude computation 
         *         Temperature : 1 = 0.1°C
         *         Pressure : 1 = 0.01mbar
         *         Altitude : 1 = 0.1m
        */
        virtual void setReferences(const int16_t ref_temperature, const uint32_t ref_pressure, const int32_t ref_altitude) = 0;

        /** \brief Get the references used for altitude computation
         *         Temperature : 1 = 0.1°C
         *         Pressure : 1 = 0.01mbar
         *         Altitude : 1 = 0.1m
        */
        virtual void getReferences(int16_t& ref_temperature, uint32_t& ref_pressure, int32_t& ref_altitude) = 0;

};

}

#endif // IBAROMETRICALTIMETER_H
