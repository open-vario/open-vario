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

#ifndef DUMMYBUZZER_H
#define DUMMYBUZZER_H

#include "IBuzzer.h"

namespace open_vario
{



/** \brief Dummy buzzer implementation */
class DummyBuzzer : public IBuzzer
{
    public:

        /** \brief Configure the buzzer */
        virtual bool configure() { return true; }

        /** \brief Set the buzzer volume : 0 = mute, 100 = max */
        virtual bool setVolume(const uint8_t volume) { (void)volume; return true; }

        /** \brief Play a note at a given frequency */
        virtual bool play(const uint32_t frequency) { (void)frequency; return true; }

        /** \brief Stop playing the note */
        virtual bool stop() { return true; }
};

}

#endif // DUMMYBUZZER_H
