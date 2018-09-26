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

#include "RunMode.h"
#include "ModeManager.h"
#include "HmiManager.h"
#include "LogMacro.h"

namespace open_vario
{


/** \brief Constructor */
RunMode::RunMode(ModeManager& mode_manager, HmiManager& hmi_manager)
: m_mode_manager(mode_manager)
, m_hmi_manager(hmi_manager)
{}

/** \brief Enter into the operating mode */
void RunMode::enter()
{
    LOG_INFO("Entering run mode...");

    // Update LED blink
    m_hmi_manager.getActivityLed().update(LedController::SLOW_BLINK);

    IOpenVarioApp::getInstance().getOs().waitMs(5000);

    // Power button pressed, switch to power off mode
    m_mode_manager.switchToMode(OPMODE_POWEROFF);
}

/** \brief Leave the operating mode */
void RunMode::leave()
{
    LOG_INFO("Leaving run mode...");
}


}
