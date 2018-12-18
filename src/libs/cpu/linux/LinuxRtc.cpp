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

#include "LinuxRtc.h"

#include <time.h>

namespace open_vario
{


/** \brief Constructor */
LinuxRtc::LinuxRtc()
{}

/** \brief Configure the RTC */
bool LinuxRtc::configure()
{
    // Nothing to do 
    return true;
}

/** \brief Set the date and time in the RTC */
bool LinuxRtc::setDateTime(const DateTime& date_time)
{
    // Convert to system time
    struct tm sys_time_tm;    
    sys_time_tm.tm_year = date_time.year + 2000u;
    sys_time_tm.tm_mon = date_time.month;
    sys_time_tm.tm_mday = date_time.day;
    sys_time_tm.tm_hour = date_time.hour;
    sys_time_tm.tm_min = date_time.minute;
    sys_time_tm.tm_sec = date_time.second;

    // Set system time
    struct timespec sys_time;
    sys_time.tv_sec = mktime(&sys_time_tm);
    sys_time.tv_nsec = 0;
    return (clock_settime(CLOCK_REALTIME, &sys_time) == 0);
}

/** \brief Get the date and time in the RTC */
bool LinuxRtc::getDateTime(DateTime& date_time)
{
    // Get system time
    struct timespec sys_time;
    clock_gettime(CLOCK_REALTIME, &sys_time);

    // Convert to DateTime
    struct tm sys_time_tm;
    localtime_r(&sys_time.tv_sec, &sys_time_tm);
    date_time.year = sys_time_tm.tm_year - 2000u;
    date_time.month = sys_time_tm.tm_mon;
    date_time.day = sys_time_tm.tm_mday;
    date_time.hour = sys_time_tm.tm_hour;
    date_time.minute = sys_time_tm.tm_min;
    date_time.second = sys_time_tm.tm_sec;

    return true;
}


}

