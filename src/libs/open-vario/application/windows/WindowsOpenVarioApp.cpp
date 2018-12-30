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

#include "WindowsOpenVarioApp.h"

#include <Windows.h>
#include <iostream>

namespace open_vario
{


/** \brief Singleton */
WindowsOpenVarioApp WindowsOpenVarioApp::m_singleton;



/** \brief Constructor */
WindowsOpenVarioApp::WindowsOpenVarioApp()
: OpenVarioApp()
, m_log_mutex()
{
    getLogger().registerListener(*this);
}

/** \brief Called when a new log is available */
void WindowsOpenVarioApp::onNewLog(const Log& log)
{
    m_log_mutex.lock();
    std::cout << log.timestamp << " - " << log.getLevelString() << " - " << log.message.cStr() << std::endl; 
    m_log_mutex.unlock();
}

/** \brief Called during application initialization */
bool WindowsOpenVarioApp::onInit(uint8_t argc, char* argv[])
{
    (void)argc;
    (void)argv;

    // Initialize sockets
    WSADATA wsa_data = {0};
    WORD version_requested = MAKEWORD(2, 2);
    const bool ret = (WSAStartup(version_requested, &wsa_data) == 0);
    return ret;
}

/** \brief Called before application start */
bool WindowsOpenVarioApp::onStart()
{
    return true;
}


/** \brief Singleton to retrieve the unique instance of the application */
IOpenVarioApp& IOpenVarioApp::getInstance()
{
    return WindowsOpenVarioApp::getInstance();
}


}
