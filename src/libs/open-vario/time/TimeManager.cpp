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

#include "TimeManager.h"
#include "LogMacro.h"
#include "nano-stl.h"
#include "OpenVarioApp.h" 
#include "OpenVarioConfig.h"
#include "ConfigManager.h"

namespace open_vario
{


/** \brief Constructor */
TimeManager::TimeManager(IRtc& rtc, ConfigManager& config_manager)
: m_rtc(rtc)
, m_config_manager(config_manager)

, m_config_values(OV_CONFIG_GROUP_DATE_TIME, "Date and time")
, m_config_time_zone(OV_CONFIG_VALUE_DATE_TIME_TZ, "Time zone", 0, -41400, 41400, false)
, m_config_gnss_synchro(OV_CONFIG_VALUE_DATE_TIME_GNSS_SYNCHRO, "GNSS synchro", true, true)

, m_time_zone(0)
, m_gnss_synchro(true)
, m_gnss_synchro_timer()
, m_gnss_date_time()
{
    // Register configuration values
    m_config_values.registerConfigValue(m_config_time_zone);
    m_config_values.registerConfigValue(m_config_gnss_synchro);
    m_config_manager.registerConfigValueGroup(m_config_values);
}


/** \brief Start the date and time manager */
bool TimeManager::start()
{
    bool ret;

    // Load configuration values
    ret = m_config_manager.getConfigValue<int32_t>(OV_CONFIG_GROUP_DATE_TIME, OV_CONFIG_VALUE_DATE_TIME_TZ, m_time_zone);
    ret = ret && m_config_manager.getConfigValue<bool>(OV_CONFIG_GROUP_DATE_TIME, OV_CONFIG_VALUE_DATE_TIME_GNSS_SYNCHRO, m_gnss_synchro);
    if (ret)
    {
        // Register to configuration changes
        ret = m_config_manager.registerConfigValueListener(OV_CONFIG_GROUP_DATE_TIME, OV_CONFIG_VALUE_DATE_TIME_TZ, *this);
    }

    // Get current date and time
    IRtc::DateTime date_time;
    if (ret)
    {
        ret = m_rtc.getDateTime(date_time);
        if (ret)
        {
            char temp[64u];
            toLocalDateTime(date_time, date_time);
            NANO_STL_SNPRINTF(temp, sizeof(temp), "Current date and time : %d/%d/%d - %d:%d:%d", date_time.day, 
                                                                                                date_time.month, 
                                                                                                (2000u + date_time.year), 
                                                                                                date_time.hour, 
                                                                                                date_time.minute, 
                                                                                                date_time.second);
            LOG_INFO(temp);
        }
    }

    // Start date and time task
    if (ret && m_gnss_synchro)
    {
        // Register to GNSS events
        m_gnss_evt_handler = NANO_STL_EVENT_HANDLER_M(TimeManager, onGnssValues, const IGnss::NavigationData&);
        ret = IOpenVarioApp::getInstance().getGnssProvider().gnssDataEvent().bind(m_gnss_evt_handler);
        if (ret)
        {
            m_gnss_date_time.millis = 0xFFFFu;
            m_gnss_synchro_timer.start(*this, 5000u);
        }
    }

    if (!ret)
    {
        LOG_ERROR("Unable to initialize Time manager");
    }    

    return ret;
}

/** \brief Set the system's date and time */
bool TimeManager::setDateTime(const IRtc::DateTime& utc_date_time, const int32_t time_zone)
{
    const bool ret = m_rtc.setDateTime(utc_date_time);
    if (ret)
    {
        m_time_zone = time_zone;
    }
    return ret;
}

/** \brief Get the system's date and time */
bool TimeManager::getDateTime(IRtc::DateTime& date_time)
{
    const bool ret = m_rtc.getDateTime(date_time);
    if (ret)
    {
        toLocalDateTime(date_time, date_time);
    }
    return true;
}

/** \brief Called when a configuration value has been modified */
void TimeManager::onConfigValueChange(const IConfigValue& config_value)
{
    m_time_zone = config_value.value<int16_t>();
}

/** \brief Method which will be called when the timer elapsed */
void TimeManager::timerElapsed(ITimer& timer)
{
    if (m_gnss_date_time.millis != 0xFFFFu)
    {
        m_rtc.setDateTime(m_gnss_date_time);
        m_gnss_synchro_timer.stop();
        IOpenVarioApp::getInstance().getGnssProvider().gnssDataEvent().unbind(m_gnss_evt_handler);
        LOG_INFO("Date and time synchronized with GNSS!");
    }
}

/** \brief Called when new GNSS values have been computed */
void TimeManager::onGnssValues(const IGnss::NavigationData& nav_data)
{
    if (nav_data.satellite_count != 0)
    {
        m_gnss_date_time = nav_data.date_time;
    }
}

/** \brief Get the local date and time corresponding to an UTC date and time */
void TimeManager::toLocalDateTime(const IRtc::DateTime& utc_date_time, IRtc::DateTime& local_date_time)
{
    /* Copy UTC date time */
    local_date_time = utc_date_time;

    /* Apply time zone */
    int32_t time_zone_hour = m_time_zone / 3600;
    if (time_zone_hour < 0)
    {
        if (local_date_time.hour >= time_zone_hour)
        {
            local_date_time.hour += time_zone_hour;
        }
        else
        {
            local_date_time.hour += (24 + time_zone_hour);
            local_date_time.day--;
            if (local_date_time.day == 0)
            {
                local_date_time.day = 31;
                local_date_time.month--;
                if (local_date_time.month == 0)
                {
                    local_date_time.month = 12;
                    local_date_time.year--;
                }
            }
        }
    }
    else
    {
        if ((local_date_time.hour + time_zone_hour) < 24)
        {
            local_date_time.hour += time_zone_hour;
        }
        else
        {
            local_date_time.hour += (time_zone_hour - 24);
            local_date_time.day++;
            if (local_date_time.day == 31)
            {
                local_date_time.day = 1;
                local_date_time.month++;
                if (local_date_time.month == 12)
                {
                    local_date_time.month = 1;
                    local_date_time.year++;
                }
            }
        }
    }
}

}
