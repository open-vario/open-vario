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

#include "DiagnosticMode.h"
#include "ModeManager.h"
#include "HmiManager.h"
#include "LogMacro.h"
#include "ICpu.h"

namespace open_vario
{


/** \brief Constructor */
DiagnosticMode::DiagnosticMode(ModeManager& mode_manager, HmiManager& hmi_manager)
: m_mode_manager(mode_manager)
, m_hmi_manager(hmi_manager)
{}

/** \brief Enter into the operating mode */
void DiagnosticMode::enter()
{
    LOG_INFO("Entering diagnostic mode...");

    // Update LED blink
    m_hmi_manager.getActivityLed().update(LedController::MEDIUM_BLINK);
}

/** \brief Leave the operating mode */
void DiagnosticMode::leave()
{
    LOG_INFO("Leaving diagnostic mode...");

    // Reset board
    IOs::getInstance().waitMs(250u);
    IOpenVarioBoard& board = IOpenVarioApp::getInstance().getBoard();
    board.cpu().reset();
}


}
