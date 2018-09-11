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

#include "LedController.h"

namespace open_vario
{


/** \brief Constructor */
LedController::LedController(ILed& led)
: m_led(led)
, m_led_mode(OFF)
, m_led_state(ILed::OFF)
, m_blink_timer()
{}

/** \brief Start the controller */
void LedController::start(const Mode led_mode)
{
    update(led_mode);
}

/** \brief Stop the controller */
void LedController::stop()
{
    if (m_led_mode >= SLOW_BLINK)
    {
        m_blink_timer.stop();
    }
    
    m_led.setOff();
}

/** \brief Update the LED state */
void LedController::update(const Mode led_mode)
{
    uint32_t blink_period = 0u;

    switch (led_mode)
    {
        case OFF:
        {
            m_led.setOff();
            m_led_state = ILed::OFF;
            break;
        }

        case ON:
        {
            m_led.setOn();
            m_led_state = ILed::ON;
            break;
        }

        case SLOW_BLINK:
        {
            blink_period = SLOW_BLINK_PERIOD;
            break;
        }

        case MEDIUM_BLINK:
        {
            blink_period = MEDIUM_BLINK_PERIOD;
            break;
        }

        case FAST_BLINK:
        {
            blink_period = FAST_BLINK_PERIOD;
            break;
        }

        default:
        {
            break;
        }
    }

    if (blink_period != 0u)
    {
        if (m_led_mode >= SLOW_BLINK)
        {
            m_blink_timer.setPeriod(blink_period);
        }
        else
        {
            m_blink_timer.start(*this, blink_period);
        }
    }
    else
    {
        if (m_led_mode >= SLOW_BLINK)
        {
            m_blink_timer.stop();
        }
    }
    m_led_mode = led_mode;
}

/** \brief Method which will be called when the timer elapsed */
void LedController::timerElapsed(ITimer& timer)
{
    if (m_led_state == ILed::ON)
    {
        m_led_state = ILed::OFF;
    }
    else
    {
        m_led_state = ILed::ON;
    }
    m_led.setState(m_led_state);
}


}
