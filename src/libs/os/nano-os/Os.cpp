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

#include "Os.h"

#include "nano_os_api.h"


namespace open_vario
{


/** \brief Singleton */
Os* Os::m_singleton = nullptr;


/** \brief Constructor */
Os::Os()
: m_os_init_suceeded(false)
{
    // OS init is done inside the constructor to allow OS objects to be construct in other application
    // objects' constructors
    const nano_os_error_t ret = NANO_OS_Init();
    m_os_init_suceeded = (ret == NOS_ERR_SUCCESS);
    m_singleton = this;
}


/** \brief Initialize the operating system */
bool Os::init()
{
    // Already done in the constructor
    return m_os_init_suceeded;
}

/** \brief Start the operating system (this method shall never return in case of success) */
bool Os::start()
{
    const nano_os_error_t ret = NANO_OS_Start();
    return (ret == NOS_ERR_SUCCESS);
}

/** \brief Get a millisecond timestamp */
uint32_t Os::getMsTimestamp()
{
    uint32_t tick_count = 0u;
    (void)NANO_OS_GetTickCount(&tick_count);
    return NANO_OS_TICKS_TO_MS(tick_count);
}

/** \brief Put the current task into sleep for a specified time duration in milliseconds */
void Os::waitMs(const uint32_t timeout)
{
    (void)NANO_OS_TASK_Sleep(NANO_OS_MS_TO_TICKS(timeout));
}

/** \brief Get the unique instance of the operating system */
IOs& IOs::getInstance()
{
    return Os::getInstance();
}

}
