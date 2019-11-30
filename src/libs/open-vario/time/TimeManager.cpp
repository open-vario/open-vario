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

namespace open_vario
{


/** \brief Constructor */
TimeManager::TimeManager(IRtc& rtc)
: m_rtc(rtc)
{}


/** \brief Start the date and time manager */
bool TimeManager::start()
{
    bool ret;

    // Get current date and time
    IRtc::DateTime date_time;
    ret = m_rtc.getDateTime(date_time);
    if (ret)
    {
        char temp[64u];
        NANO_STL_SNPRINTF(temp, sizeof(temp), "Current date and time : %d/%d/%d - %d:%d:%d", date_time.day, 
                                                                                             date_time.month, 
                                                                                             (2000u + date_time.year), 
                                                                                             date_time.hour, 
                                                                                             date_time.minute, 
                                                                                             date_time.second);
        LOG_INFO(temp);
    }
    else
    {
        LOG_ERROR("Unable to initialize Time manager");
    }    

    return ret;
}

/** \brief Set the system's date and time */
bool TimeManager::setDateTime(const IRtc::DateTime& date_time)
{
    return m_rtc.setDateTime(date_time);
}

/** \brief Get the system's date and time */
bool TimeManager::getDateTime(IRtc::DateTime& date_time)
{
    return m_rtc.getDateTime(date_time);
}


}
