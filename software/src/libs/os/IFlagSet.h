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

#ifndef IFLAGSET
#define IFLAGSET

#include <cstdint>

namespace open_vario
{


/** \brief Interface for all flag set implementations */
class IFlagSet
{
    public:

        /** \brief Reset flags in the flag set */
        virtual bool reset(const uint32_t flag_mask) = 0;

        /** \brief Set flags in the flag set */
        virtual bool set(const uint32_t flag_mask, const bool from_isr) = 0;

        /** \brief Wait for flags in the flag set */
        virtual bool wait(uint32_t& flag_mask, const bool reset_flags, const uint32_t timeout) = 0;
};


}

#endif // IFLAGSET
