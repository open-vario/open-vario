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

#include "PwmBuzzer.h"


namespace open_vario
{

/** \brief Constructor */
PwmBuzzer::PwmBuzzer(IPwm& pwm)
: m_pwm(pwm)
{}

/** \brief Configure the buzzer */
bool PwmBuzzer::configure()
{
    // Default PWM configuration => mute
    return m_pwm.configure(100u, 100u);
}

/** \brief Set the buzzer volume : 0 = mute, 100 = max */
bool PwmBuzzer::setVolume(const uint8_t volume)
{
    bool ret = false;

    // Check input range
    if (volume <= 100u)
    {
        // Convert volume into duty cycle
        // Max volume => 50% PWM duty cycle
        // Mute => 100% PWM duty cycle
        uint32_t duty_cycle = static_cast<uint32_t>(volume) * 50u / 100u;
        if (duty_cycle == 0)
        {
            duty_cycle = 100u;
        }

        // Apply duty cycle
        ret = m_pwm.setDutyCycle(static_cast<uint8_t>(duty_cycle));
    }

    return ret;
}

/** \brief Play a note at a given frequency */
bool PwmBuzzer::play(const uint32_t frequency)
{
    // Configure frequency
    bool ret = m_pwm.setFrequency(frequency);
    if (ret)
    {
        // Start playing
        ret = m_pwm.start();
    }
    return ret;
}

/** \brief Stop playing the note */
bool PwmBuzzer::stop()
{
    // Stop PWM
    return m_pwm.stop();
}

}
