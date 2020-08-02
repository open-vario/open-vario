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

#include "ButtonController.h"
#include "IOs.h"

namespace open_vario
{


/** \brief Constructor */
ButtonController::ButtonController(IInputPin& pin, const IIoPin::Level released_level)
: m_pin(pin)
, m_released_level(released_level)
, m_timer()
, m_previous_level(m_released_level)
, m_push_start(0)
{}

/** \brief Start the controller */
void ButtonController::start()
{
    m_previous_level = m_released_level;
    m_timer.start(*this, POOLING_PERIOD);
}

/** \brief Stop the controller */
void ButtonController::stop()
{
    m_timer.stop();
}

/** \brief Method which will be called when the timer elapsed */
void ButtonController::timerElapsed(ITimer& timer)
{
    const IIoPin::Level pin_level = m_pin.getLevel();
    const uint32_t now = IOs::getInstance().getMsTimestamp();

    if (m_previous_level == m_released_level)
    {
        if (pin_level != m_released_level)
        {
            m_push_start = now;
        }
    }
    else
    {
        if (pin_level == m_released_level)
        {
           if ((now - m_push_start) >= SHORT_PUSH_DURATION)
           {
               
           }
        }
    }
    

    m_previous_level = pin_level;
}


}
