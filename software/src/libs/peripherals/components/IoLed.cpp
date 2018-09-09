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

#include "IoLed.h"
#include "IOutputPin.h"

namespace open_vario
{


/** \brief Constructor */
IoLed::IoLed(IOutputPin& pin, const State initial_state, const IIoPin::Level on_state_level)
: m_pin(pin)
, m_initial_state(initial_state)
, m_on_state_level(on_state_level)
{}


/** \brief Configure the LED */
bool IoLed::configure()
{
    return m_pin.configure();
}

/** \brief Set LED on */
void IoLed::setOn()
{
    m_pin.setLevel(m_on_state_level);
}

/** \brief Set LED off */
void IoLed::setOff()
{
    m_pin.setLevel(!m_on_state_level);
}

/** \brief Set the LED to a specified state */
void IoLed::setState(const State state)
{
    if (state == ON)
    {
        setOn();
    }
    else
    {
        setOff();
    }
}


}
