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

#ifndef MUTEX_H
#define MUTEX_H

#include "IMutex.h"

#include <Windows.h>

namespace open_vario
{


/** \brief Mutex implementation for Windows */
class Mutex : public IMutex
{
    public:

        /** \brief Constructor */
        Mutex();


        /** \brief Lock the mutex */
        virtual bool lock();

        /** \brief Unlock the mutex */
        virtual bool unlock();
        

    private:

        /** \brief Underlying Windows critical section */
        CRITICAL_SECTION m_cs;

};

}

#endif // MUTEX_H
