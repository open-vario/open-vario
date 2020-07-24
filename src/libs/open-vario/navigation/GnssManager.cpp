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

#include "GnssManager.h"
#include "OpenVarioConfig.h"
#include "OpenVarioTasks.h"
#include "OpenVarioFaults.h"
#include "IOpenVarioApp.h"
#include "LogMacro.h"


namespace open_vario
{


/** \brief Constructor */
GnssManager::GnssManager(ConfigManager& config_manager, FaultManager& fault_manager, IGnss& gnss)
: m_config_manager(config_manager)
, m_fault_manager(fault_manager)
, m_gnss(gnss)

, m_config_values(OV_CONFIG_GROUP_GNSS, "GNSS")
, m_config_enabled(OV_CONFIG_VALUE_GNSS_ENABLED, "GNSS enabled", true, true)
, m_config_acq_period(OV_CONFIG_VALUE_GNSS_ACQ_PERIOD, "Acquisition period", 1000u, 500u, 5000u, true)

, m_enabled(true)
, m_acq_period(0u)
, m_task("GNSS task", OV_TASK_PRIO_GNSS_MANAGER)
, m_acq_timer()
, m_acq_trigger_sem(0u, 1u)
{
    // Register configuration values
    m_config_values.registerConfigValue(m_config_enabled);
    m_config_values.registerConfigValue(m_config_acq_period);
    m_config_manager.registerConfigValueGroup(m_config_values);
}

/** \brief Initialize the GNSS manager */
bool GnssManager::init()
{
    bool ret;

    LOG_INFO("Initializing GNSS...");
    
    // Load configuration values
    ret = m_config_manager.getConfigValue<bool>(OV_CONFIG_GROUP_GNSS, OV_CONFIG_VALUE_GNSS_ENABLED, m_enabled);
    ret = ret && m_config_manager.getConfigValue<uint16_t>(OV_CONFIG_GROUP_GNSS, OV_CONFIG_VALUE_GNSS_ACQ_PERIOD, m_acq_period);
    if (ret && m_enabled)
    {
        // Initialize GNSS
        ret = m_gnss.configure(m_acq_period);
        if (!ret)
        {
            m_fault_manager.raise(OV_FAULT_GNSS_KO);
            LOG_ERROR("Unable to configure GNSS");
        }        
    }

    return ret;
}

/** \brief Start the GNSS manager */
bool GnssManager::start()
{
    bool ret = true;

    // Check if the module is enabled
    if (m_enabled)
    {
        LOG_INFO("Starting GNSS acquisition...");

        // Start GNSS
        ret = m_gnss.start();
    }

    return ret;
}

/** \brief Start the GNSS notifications */
bool GnssManager::startNotifications()
{
    bool ret = true;

    // Check if the module is enabled
    if (m_enabled)
    {
        LOG_INFO("Starting GNSS notifications...");

        // Start the acquisition timer
        ret = m_acq_timer.start(*this, m_acq_period);

        // Start the task
        ITask::TaskMethod task_method = TASK_METHOD(GnssManager, task);
        ret = ret && m_task.start(task_method, nullptr);
    }

    return ret;
}

/** \brief Sensors manager's task method */
void GnssManager::task(void* const param)
{
    (void)param;

    IGnss::NavigationData nav_data = {0};

    // Task loop
    while (true)
    {
        // Wait for acquisition trigger
        if (m_acq_trigger_sem.wait(IOpenVarioApp::getInstance().getOs().getInfiniteTimeoutValue()))
        {

            // Get navigation data
            if (m_gnss.readData(nav_data))
            {
                // Check navigation data validity
                if (nav_data.satellite_count == 0)
                {
                    if (!m_fault_manager.isActive(OV_FAULT_GNSS_NO_FIX))
                    {
                        m_fault_manager.raise(OV_FAULT_GNSS_NO_FIX);
                        LOG_ERROR("GNSS fix lost");
                    }
                    memset(&nav_data, 0, sizeof(nav_data));
                }
                else
                {
                    if (m_fault_manager.isActive(OV_FAULT_GNSS_NO_FIX))
                    {
                        m_fault_manager.clear(OV_FAULT_GNSS_NO_FIX);
                        LOG_ERROR("GNSS fix acquired");
                    }
                }
            }
            else
            {
                // GNSS failure
                if (!m_fault_manager.isActive(OV_FAULT_GNSS_KO))
                {
                    m_fault_manager.raise(OV_FAULT_GNSS_KO);
                    LOG_ERROR("Unable to read GNSS data");
                }
                memset(&nav_data, 0, sizeof(nav_data));
            }

            // Notify navigation data
            m_gnss_data_event.trigger(nav_data);
        }
    }
}

/** \brief Method which will be called when the timer elapsed */
void GnssManager::timerElapsed(ITimer& timer)
{
    (void)timer;
    m_acq_trigger_sem.post(false);
}

/** \brief Get the current navigation data */
bool GnssManager::getData(IGnss::NavigationData& nav_data)
{
    return m_gnss.readData(nav_data);
}


}
