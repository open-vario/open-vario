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

#include <Windows.h>


namespace open_vario
{


/** \brief Singleton */
Os* Os::m_singleton = NULL;


/** \brief Constructor */
Os::Os()
{
    m_singleton = this;
}


/** \brief Initialize the operating system */
bool Os::init()
{
    return true;
}

/** \brief Start the operating system (this method shall never return in case of success) */
bool Os::start()
{
    Sleep(INFINITE);
    return false;
}

/** \brief Get a millisecond timestamp */
uint32_t Os::getMsTimestamp()
{
    return static_cast<uint32_t>(GetTickCount());
}

/** \brief Put the current task into sleep for a specified time duration in milliseconds */
void Os::waitMs(const uint32_t timeout)
{
    Sleep(timeout);
}

/** \brief Get the unique instance of the operating system */
IOs& IOs::getInstance()
{
    return Os::getInstance();
}

}
