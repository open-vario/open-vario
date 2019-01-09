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

#ifndef IVARIOMETER_H
#define IVARIOMETER_H

#include "IEvent.h"

namespace open_vario
{


/** \brief Variometer output values */
struct VariometerValues
{
    /** \brief Vario */
    int16_t vario;
    /** \brief Minimum vario */
    int16_t min_vario;
    /** \brief Maximum vario */
    int16_t max_vario;
};


/** \brief Interface for all variometers implementations */
class IVariometer
{
    public:

        /** \brief Event triggered on new variometer values */
        virtual nano_stl::IEvent<const VariometerValues&>& variometerValuesEvent() = 0;

};

}

#endif // IVARIOMETER_H
