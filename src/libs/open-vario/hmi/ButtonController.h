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

#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

#include "Timer.h"
#include "IInputPin.h"

namespace open_vario
{


/** \brief Control a button */
class ButtonController : public ITimerListener
{
    public:


        /** \brief Button events */
        enum Event
        {
            /** \brief Pressed */
            PRESSED = 0u,
            /** \brief Released */
            RELEASED = 1u,
            /** \brief Short push */
            SHORT_PUSH = 2u,
            /** \brief Long push */
            LONG_PUSH = 3u
        };


        /** \brief Constructor */
        ButtonController(IInputPin& pin, const IIoPin::Level released_level);


        /** \brief Start the controller */
        void start();

        /** \brief Stop the controller */
        void stop();


        /** \brief Method which will be called when the timer elapsed */
        virtual void timerElapsed(ITimer& timer);

    private:

        /** \brief Input pin of the button */
        IInputPin& m_pin;

        /** \brief Pin level for the button released state */
        const IIoPin::Level m_released_level;

        /** \brief Timer to handle the button */
        Timer m_timer;

        /** \brief Previous button state */
        IIoPin::Level m_previous_level;

        /** \brief Push start timestamp */
        uint32_t m_push_start;


        /** \brief Pooling period in milliseconds */
        static const uint32_t POOLING_PERIOD = 50u;

        /** \brief Short push duration in milliseconds */
        static const uint32_t SHORT_PUSH_DURATION = 250u;

        /** \brief Long push duration in milliseconds */
        static const uint32_t LONG_PUSH_DURATION = 2000u;

};

}

#endif // BUTTONCONTROLLER_H
