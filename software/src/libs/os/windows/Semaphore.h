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

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "ISemaphore.h"

#include <Windows.h>

namespace open_vario
{


/** \brief Semaphore implementation for Windows */
class Semaphore : public ISemaphore
{
    public:

        /** \brief Constructor */
        Semaphore(const uint32_t init_count, const uint32_t max_count);


        /** \brief Post a token into the semaphore */
        virtual bool post(const bool from_isr);

        /** \brief Wait for a token in the semaphore  */
        virtual bool wait(const uint32_t timeout);
        

    private:

        /** \brief Underlying Windows semaphore */
        HANDLE m_sem;

};

}

#endif // SEMAPHORE_H
