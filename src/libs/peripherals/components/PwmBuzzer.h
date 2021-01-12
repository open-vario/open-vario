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

#ifndef PWMBUZZER_H
#define PWMBUZZER_H

#include "IBuzzer.h"
#include "IPwm.h"
#include "IOutputPin.h"


namespace open_vario
{

/** \brief Buzzer driven by a PWM signal */
class PwmBuzzer : public IBuzzer
{
    public:

        /** \brief Constructor */
        PwmBuzzer(IPwm& pwm, IOutputPin& power_pin);


        /** \brief Configure the buzzer */
        virtual bool configure();

        /** \brief Set the buzzer volume : 0 = mute, 100 = max */
        virtual bool setVolume(const uint8_t volume);

        /** \brief Play a note at a given frequency */
        virtual bool play(const uint32_t frequency);

        /** \brief Stop playing the note */
        virtual bool stop();

    
    private:

        /** \brief PWM driver */
        IPwm& m_pwm;

        /** \brief Buzzer power enable driver */
        IOutputPin& m_power_pin;
};

}

#endif // PWMBUZZER_H
