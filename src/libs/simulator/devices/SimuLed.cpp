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

#include "SimuLed.h"

using namespace std;

namespace open_vario
{


/** \brief Constructor */
SimuLed::SimuLed(ISimulator& simulator, const std::string& name, const State initial_state)
: SimuDevice(simulator, type(), name)
, m_initial_state(initial_state)
, m_state(initial_state)
{}


/** \brief Configure the LED */
bool SimuLed::configure()
{
    return true;
}

/** \brief Set LED on */
void SimuLed::setOn()
{
    m_state = ON;
}

/** \brief Set LED off */
void SimuLed::setOff()
{
    m_state = OFF;
}

/** \brief Set the LED to a specified state */
void SimuLed::setState(const State state)
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
