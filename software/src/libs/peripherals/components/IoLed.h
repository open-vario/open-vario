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

#ifndef IOLED_H
#define IOLED_H

#include "ILed.h"
#include "IIoPin.h"

namespace open_vario
{

class IOutputPin;


/** \brief LED driven by an IO */
class IoLed : public ILed
{
    public:

        /** \brief Constructor */
        IoLed(IOutputPin& pin, const State initial_state, const IIoPin::Level on_state_level);


        /** \brief Configure the LED */
        virtual bool configure();

        /** \brief Set LED on */
        virtual void setOn();

        /** \brief Set LED off */
        virtual void setOff();

        /** \brief Set the LED to a specified state */
        virtual void setState(const State state);

    private:

        /** \brief Output pin to drive the LED */
        IOutputPin& m_pin;

        /** \brief Initial state */
        State m_initial_state;

        /** \brief Pin level for LED on state */
        IIoPin::Level m_on_state_level;
};

}

#endif // IOLED_H
