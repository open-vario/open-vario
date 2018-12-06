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

#ifndef ITEMPERATURESENSOR_H
#define ITEMPERATURESENSOR_H

#include <cstdint>

namespace open_vario
{



/** \brief Interface for all temperature sensors implementations */
class ITemperatureSensor
{
    public:

        /** \brief Configure the temperature sensor */
        virtual bool configure() = 0;

        /** \brief Read the temperature (1 = 0.1°C) */
        virtual bool readTemperature(int16_t& temperature) = 0;
};

}

#endif // ITEMPERATURESENSOR_H
