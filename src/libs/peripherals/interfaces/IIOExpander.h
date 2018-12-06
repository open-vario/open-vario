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

#ifndef IIOEXPANDER_H
#define IIOEXPANDER_H

#include "IIoPin.h"

namespace open_vario
{



/** \brief Interface for all I/O expanders implementations */
class IIOExpander
{
    public:

        /** \brief Configure the I/O expander */
        virtual bool configure() = 0;

        /** \brief Configure the mode of an I/O */
        virtual bool configurePin(const uint8_t pin, const bool input) = 0;

        /** \brief Set the level of an I/O */
        virtual void setLevel(const uint8_t pin, const IIoPin::Level level) = 0;

        /** \brief Get the level of an I/O */
        virtual IIoPin::Level getLevel(const uint8_t pin) = 0;
};

}

#endif // IIOEXPANDER_H
