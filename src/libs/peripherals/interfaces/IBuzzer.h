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

#ifndef IBUZZER_H
#define IBUZZER_H

#include <cstdint>

namespace open_vario
{



/** \brief Interface for all buzzers implementations */
class IBuzzer
{
    public:

        /** \brief Configure the buzzer */
        virtual bool configure() = 0;

        /** \brief Set the buzzer volume : 0 = mute, 100 = max */
        virtual bool setVolume(const uint8_t volume) = 0;

        /** \brief Play a note at a given frequency */
        virtual bool play(const uint32_t frequency) = 0;

        /** \brief Stop playing the note */
        virtual bool stop() = 0;
};

}

#endif // IBUZZER_H
