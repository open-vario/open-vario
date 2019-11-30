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

#include "SimuBuzzer.h"

using namespace std;

namespace open_vario
{


/** \brief Constructor */
SimuBuzzer::SimuBuzzer(ISimulator& simulator, const std::string& name)
: SimuDevice(simulator, type(), name)
, m_volume(50u)
, m_frequency(0u)
, m_playing(false)
{}


/** \brief Configure the buzzer */
bool SimuBuzzer::configure()
{
    return true;   
}

/** \brief Set the buzzer volume : 0 = mute, 100 = max */
bool SimuBuzzer::setVolume(const uint8_t volume)
{
    m_volume = volume;
    return true;
}

/** \brief Play a note at a given frequency */
bool SimuBuzzer::play(const uint32_t frequency)
{
    m_frequency = frequency;
    m_playing = true;
    return true;
}

/** \brief Stop playing the note */
bool SimuBuzzer::stop()
{
    m_playing = false;
    return true;
}


}
