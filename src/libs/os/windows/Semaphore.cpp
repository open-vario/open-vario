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

#include "Semaphore.h"

namespace open_vario
{


/** \brief Constructor */
Semaphore::Semaphore(const uint32_t init_count, const uint32_t max_count)
: m_sem(NULL)
{
    m_sem = CreateSemaphore(NULL, static_cast<LONG>(init_count), static_cast<LONG>(max_count), NULL);
}


/** \brief Post a token into the semaphore */
bool Semaphore::post(const bool from_isr)
{
    (void)from_isr;

    BOOL ret = ReleaseSemaphore(&m_sem, 1, NULL);
    return (ret != 0); 
}

/** \brief Wait for a token in the semaphore  */
bool Semaphore::wait(const uint32_t timeout)
{
    DWORD ret = WaitForSingleObject(m_sem, static_cast<DWORD>(timeout));
    return (ret == WAIT_OBJECT_0);
}


}
