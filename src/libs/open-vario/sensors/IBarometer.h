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

#ifndef IBAROMETER_H
#define IBAROMETER_H

#include "IEvent.h"

namespace open_vario
{


/** \brief Barometer output values */
struct BarometerValues
{
    /** \brief Pressure */
    uint32_t pressure;
    /** \brief Minimum pressure */
    uint32_t min_pressure;
    /** \brief Maximum pressure */
    uint32_t max_pressure;
};


/** \brief Interface for all barometers implementations */
class IBarometer
{
    public:

        /** \brief Event triggered on new barometer values */
        virtual nano_stl::IEvent<const BarometerValues&>& barometerValuesEvent() = 0;

};

}

#endif // IBAROMETER_H
