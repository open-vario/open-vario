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

#include "NanoOsOpenVarioApp.h"

#include "nano_os_api.h"

namespace open_vario
{


/** \brief Singleton */
NanoOsOpenVarioApp NanoOsOpenVarioApp::m_singleton;



/** \brief Constructor */
NanoOsOpenVarioApp::NanoOsOpenVarioApp()
: OpenVarioApp()
, m_nano_os_console(getBoard().debugUart(), getLogHistory())
{}

/** \brief Called during application initialization */
bool NanoOsOpenVarioApp::onInit(uint8_t argc, char* argv[])
{
    (void)argc;
    (void)argv;

    // Initialize Nano-OS console commands
    bool ret = m_nano_os_console.init();

    return ret;
}

/** \brief Called before application start */
bool NanoOsOpenVarioApp::onStart()
{
    return true;
}


/** \brief Singleton to retrieve the unique instance of the application */
IOpenVarioApp& IOpenVarioApp::getInstance()
{
    return NanoOsOpenVarioApp::getInstance();
}


}
