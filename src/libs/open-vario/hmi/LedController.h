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

#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include "Timer.h"
#include "ILed.h"

namespace open_vario
{


/** \brief Control a LED state */
class LedController : public ITimerListener
{
    public:


        /** \brief LED mode */
        enum Mode
        {
            /** \brief Off */
            OFF = 0u,
            /** \brief On */
            ON = 1u,
            /** \brief Slow blink */
            SLOW_BLINK = 2u,
            /** \brief Medium blink */
            MEDIUM_BLINK = 3u,
            /** \brief Fast blink */
            FAST_BLINK = 4u,
            /** \brief Slow pulse */
            SLOW_PULSE = 5u,
            /** \brief Medium pulse */
            MEDIUM_PULSE = 6u,
            /** \brief Fast pulse */
            FAST_PULSE = 7u
        };


        /** \brief Constructor */
        LedController(ILed& led);


        /** \brief Start the controller */
        void start(const Mode led_mode);

        /** \brief Stop the controller */
        void stop();

        /** \brief Update the LED state */
        void update(const Mode led_mode);


        /** \brief Method which will be called when the timer elapsed */
        virtual void timerElapsed(ITimer& timer);

    private:

        /** \brief LED to control */
        ILed& m_led;

        /** \brief LED mode */
        Mode m_led_mode;

        /** \brief LED state */
        ILed::State m_led_state;

        /** \brief Timer to handle the blinking */
        Timer m_blink_timer;

        /** \brief Blink period in milliseconds */
        uint32_t m_blink_period;



        /** \brief Slow blink period in milliseconds */
        static const uint32_t SLOW_BLINK_PERIOD = 1000u;

        /** \brief Medium blink period in milliseconds */
        static const uint32_t MEDIUM_BLINK_PERIOD = 500u;

        /** \brief Fast blink period in milliseconds */
        static const uint32_t FAST_BLINK_PERIOD = 100u;

        /** \brief Pulse length in milliseconds */
        static const uint32_t PULSE_LENGTH = 50u;

};

}

#endif // LEDCONTROLLER_H
