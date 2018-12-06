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

#include "OpenVarioApp.h"

namespace open_vario
{


/** \brief Constructor */
OpenVarioApp::OpenVarioApp()
: m_os()
, m_board()

, m_log_manager(Log::LL_INFO, m_os)
, m_log_history(m_log_manager)

, m_mode_manager(m_operating_modes)
, m_operating_modes()
, m_init_mode(m_mode_manager, m_hmi_manager, m_time_manager)
, m_run_mode(m_mode_manager, m_hmi_manager)
, m_power_off_mode(m_mode_manager, m_hmi_manager)

, m_hmi_manager(m_board.activityLed())

, m_time_manager(m_board.rtc())
{
    m_operating_modes.pushBack(&m_init_mode);
    m_operating_modes.pushBack(&m_run_mode);
    m_operating_modes.pushBack(&m_power_off_mode);
}

/** \brief Initialize the application */
bool OpenVarioApp::init(uint8_t argc, char* argv[])
{
    bool ret = false;

    // Initialize operating system
    if (m_os.init())
    {
        // Initialize application
        ret = onInit(argc, argv);
    }

    return ret;
}

/** \brief Start the application */
bool OpenVarioApp::start()
{
    bool ret = false;

    // Start application
    if (onStart())
    {
        // Start mode manager
        if (m_mode_manager.start())
        {
            // Start operating system
            ret = m_os.start();
        }
    }

    return ret;
}


}
