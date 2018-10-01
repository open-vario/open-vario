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

#ifndef IBAROMETRICSENSOR_H
#define IBAROMETRICSENSOR_H

#include "ITemperatureSensor.h"

namespace open_vario
{



/** \brief Interface for all barometric sensors implementations */
class IBarometricSensor : public ITemperatureSensor
{
    public:

        /** \brief Read the pressure (1 = 0.01mbar) */
        virtual bool readPressure(uint32_t& pressure) = 0;
};

}

#endif // IBAROMETRICSENSOR_H
