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

#include "WindowsCpu.h"

#include <Windows.h>

namespace open_vario
{


/** \brief Constructor */
WindowsCpu::WindowsCpu()
{}

/** \brief Reset the CPU */
bool WindowsCpu::reset()
{
    // Restart a new instance of the application
    char filename[512u];
    GetModuleFileNameA(NULL, filename, sizeof(filename));

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    CreateProcess(filename, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    // Exit the current instance of the application
    exit(EXIT_SUCCESS);

    // If we get here then something went wrong!!
    return false;
}


}

