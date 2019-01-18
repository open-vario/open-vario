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

#include "InitMode.h"
#include "ModeManager.h"
#include "HmiManager.h"
#include "TimeManager.h"
#include "ConfigManager.h"
#include "SensorsManager.h"
#include "LogMacro.h"
#include "ISpi.h"


namespace open_vario
{


/** \brief Constructor */
InitMode::InitMode(ModeManager& mode_manager, HmiManager& hmi_manager, TimeManager& time_manager, ConfigManager& config_manager, SensorsManager& sensors_manager)
: m_mode_manager(mode_manager)
, m_hmi_manager(hmi_manager)
, m_time_manager(time_manager)
, m_config_manager(config_manager)
, m_sensors_manager(sensors_manager)
{}

/** \brief Enter into the operating mode */
void InitMode::enter()
{
    // Initialize the board
    IOpenVarioBoard& board = IOpenVarioApp::getInstance().getBoard();
    bool ret = board.configure();
    if (!ret)
    {
        LOG_ERROR("Failure during board initialization...");
    }
    else
    {
        LOG_INFO("Entering init mode...");

        // Start HMI
        m_hmi_manager.start();
        m_hmi_manager.getActivityLed().update(LedController::FAST_BLINK);

        // Start date and time management
        m_time_manager.start();

        // Initialize configuration
        m_config_manager.init();    

        // Start the board peripherals
        ret = board.start();
        if (ret)
        {
            // Initialize and start sensor manager
            ret = m_sensors_manager.init();
            ret = ret && m_sensors_manager.start();
            if (ret)
            {
                // Blink a bit :)
                IOpenVarioApp::getInstance().getOs().waitMs(2000u);
            }
            else
            {
                LOG_ERROR("Failure during sensors startup...");
            }
        }
        else
        {
            LOG_ERROR("Failure during board startup...");
        }

        // Change mode
        if (ret)
        {
            // Init done, switch to run mode
            m_mode_manager.switchToMode(OPMODE_RUN);
        } 
        else
        {
            // Init failed, switch to power off mode
            m_mode_manager.switchToMode(OPMODE_POWEROFF);
        }
    }
}

/** \brief Leave the operating mode */
void InitMode::leave()
{
    LOG_INFO("Leaving init mode...");
}


}
