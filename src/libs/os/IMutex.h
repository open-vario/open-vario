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

#ifndef IMUTEX_H
#define IMUTEX_H

#include <cstdint>

namespace open_vario
{



/** \brief Interface for all mutex implementations */
class IMutex
{
    public:

        /** \brief Lock the mutex */
        virtual bool lock() = 0;

        /** \brief Unlock the mutex */
        virtual bool unlock() = 0;
};

}

#endif // IMUTEX_H
