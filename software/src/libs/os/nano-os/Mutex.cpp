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

#include "Mutex.h"

namespace open_vario
{


/** \brief Constructor */
Mutex::Mutex()
: m_mutex()
{
    NANO_OS_MUTEX_Create(&m_mutex, QT_PRIORITY);
}


/** \brief Lock the mutex */
bool Mutex::lock()
{
    const nano_os_error_t ret = NANO_OS_MUTEX_Lock(&m_mutex, 0xFFFFFFFFu);
    return (ret == NOS_ERR_SUCCESS);
}

/** \brief Unlock the mutex */
bool Mutex::unlock()
{
    const nano_os_error_t ret = NANO_OS_MUTEX_Unlock(&m_mutex);
    return (ret == NOS_ERR_SUCCESS);
}


}
