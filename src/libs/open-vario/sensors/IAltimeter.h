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

#ifndef IALTIMETER_H
#define IALTIMETER_H

#include <cstdint>

namespace open_vario
{

/** \brief Altimeter output values */
struct AltimeterValues
{
    /** \brief Main altitude */
    int32_t main_alti;
    /** \brief Altitude 1 */
    int32_t alti_1;
    /** \brief Altitude 2 */
    int32_t alti_2;
    /** \brief Altitude 3 */
    int32_t alti_3;
    /** \brief Altitude 4 */
    int32_t alti_4;
    /** \brief Minimum altitude */
    int32_t min_alti;
    /** \brief Maximum altitude */
    int32_t max_alti;
};


/** \brief Interface for all altimeters listeners implementations */
class IAltimeterListener
{
    public:

        /** \brief Called when new altimeter values have been computed */
        virtual void onAltimeterValues(const AltimeterValues& alti_values) = 0;
};


/** \brief Interface for all altimeters implementations */
class IAltimeter
{
    public:

        /** \brief Set a reference altitude (1 = 0.1m) */
        virtual bool setReferenceAltitude(const int32_t ref_altitude) = 0;

        /** \brief Register a listener for the altimeter values */
        virtual bool registerListener(IAltimeterListener& listener) = 0;

};

}

#endif // IALTIMETER_H
