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

#ifndef ILED_H
#define ILED_H

#include <cstdint>

namespace open_vario
{



/** \brief Interface for all LED implementations */
class ILed
{
    public:

        /** \brief Led state */
        typedef bool State;
        /** \brief Led ON */
        static const State ON = true;
        /** \brief Led OFF */
        static const State OFF = false;


        /** \brief Configure the LED */
        virtual bool configure() = 0;

        /** \brief Set LED on */
        virtual void setOn() = 0;

        /** \brief Set LED off */
        virtual void setOff() = 0;

        /** \brief Set the LED to a specified state */
        virtual void setState(const State state) = 0;
};

}

#endif // ILED_H
