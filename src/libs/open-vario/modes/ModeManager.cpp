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
#include "OpenVarioTasks.h"
#include "HmiManager.h"
#include "LogMacro.h"

namespace open_vario
{


/** \brief Constructor */
ModeManager::ModeManager(nano_stl::IArray<IMode*>& operating_modes)
: m_task("Mode manager", OV_TASK_PRIO_MODE_MANAGER)
, m_operating_modes(operating_modes)
, m_mode_change_queue()
{}

/** \brief Start the mode manager */
bool ModeManager::start()
{
    ITask::TaskMethod task_method = TASK_METHOD(ModeManager, task);
    return m_task.start(task_method, nullptr);
}

/** \brief Switch to the requested mode */
void ModeManager::switchToMode(const OperatingMode mode)
{
    m_mode_change_queue.post(mode, false);
}

/** \brief Mode manager's task method */
void ModeManager::task(void* const param)
{
    (void)param;

    LOG_INFO("----------------------------------------");
    LOG_INFO("Starting Open Vario v%s", IOpenVarioApp::getInstance().getVersion());
    LOG_INFO("----------------------------------------");

    // Enter first mode
    IMode* current_mode = m_operating_modes[0u];
    current_mode->enter();

    // Task loop    
    while (true)
    {
        // Wait for a change of mode
        OperatingMode mode;
        if (m_mode_change_queue.wait(mode, IOs::getInstance().getInfiniteTimeoutValue()))
        {
            IMode* const new_mode = m_operating_modes[mode];
            if (new_mode != current_mode)
            {
                current_mode->leave();
                new_mode->enter();
                current_mode = new_mode;
            }
        }
    }

}


}
