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

#include "ModeManager.h"
#include "IOpenVarioApp.h"
#include "HmiManager.h"

namespace open_vario
{


/** \brief Constructor */
ModeManager::ModeManager(HmiManager& hmi_manager)
: m_task("Mode manager", 10u)
, m_hmi_manager(hmi_manager)
{}

/** \brief Start the mode manager */
bool ModeManager::start()
{
    return m_task.start(*this, NULL);
}

/** \brief Method which will be called at the task's startup */
void ModeManager::taskStart(void* const param)
{
    (void)param;

    m_hmi_manager.start();

    // Task loop
    while (true)
    {
        IOpenVarioApp::getInstance().getOs().waitMs(1000);
    }

}


}
