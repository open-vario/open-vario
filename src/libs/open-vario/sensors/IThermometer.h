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

#ifndef ITHERMOMETER_H
#define ITHERMOMETER_H

#include "IEvent.h"

namespace open_vario
{


/** \brief Thermometer output values */
struct ThermometerValues
{
    /** \brief Temperature */
    int16_t temperature;
    /** \brief Minimum temperature */
    int16_t min_temperature;
    /** \brief Maximum temperature */
    int16_t max_temperature;
};


/** \brief Interface for all thermometers implementations */
class IThermometer
{
    public:

        /** \brief Reset the min and max values */
        virtual void resetMinMax() = 0;

        /** \brief Event triggered on new thermometer values */
        virtual nano_stl::IEvent<const ThermometerValues&>& thermometerValuesEvent() = 0;

};

}

#endif // ITHERMOMETER_H
