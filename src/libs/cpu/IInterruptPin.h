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

#ifndef IINTERRUPTPIN_H
#define IINTERRUPTPIN_H

#include "IIoPin.h"

namespace open_vario
{


/** \brief Interface for all interrupt pins listeners implementations */
class IInterruptPinListener
{
    public:

        /** \brief Called when the interrupt on the pin is triggered */
        virtual void onPinInterrupt(const IIoPin::Level level) = 0;

};



/** \brief Interface for all interrupt pins implementations */
class IInterruptPin : public IIoPin
{
    public:

        /** \brief Configure the pin */
        virtual bool configure() = 0;

        /** \brief Register a listener for the interrupt event */
        virtual bool registerListener(IInterruptPinListener& listener) = 0;

        /** \brief Enable interrupt on the pin */
        virtual bool enableInterrupt() = 0;

        /** \brief Disable interrupt on the pin */
        virtual bool disableInterrupt() = 0;

};

}

#endif // IINTERRUPTPIN_H
