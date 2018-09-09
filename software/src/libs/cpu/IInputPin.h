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

#ifndef IINPUTPIN_H
#define IINPUTPIN_H

#include "IIoPin.h"

namespace open_vario
{



/** \brief Interface for all input pins implementations */
class IInputPin : public IIoPin
{
    public:

        /** \brief Configure the pin */
        virtual bool configure() = 0;

        /** \brief Indicate if the pin level is low */
        virtual bool isLow() = 0;

        /** \brief Indicate if the pin level is high */
        virtual bool isHigh() = 0;

        /** \brief Get the pin level */
        virtual Level getLevel() = 0;

};

}

#endif // IINPUTPIN_H
