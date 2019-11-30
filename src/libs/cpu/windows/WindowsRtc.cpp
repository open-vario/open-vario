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

#include "WindowsRtc.h"

#include <Windows.h>

namespace open_vario
{


/** \brief Constructor */
WindowsRtc::WindowsRtc()
{}

/** \brief Configure the RTC */
bool WindowsRtc::configure()
{
    // Nothing to do 
    return true;
}

/** \brief Set the date and time in the RTC */
bool WindowsRtc::setDateTime(const DateTime& date_time)
{
    // Convert to system time
    SYSTEMTIME sys_time;
    sys_time.wYear = date_time.year + 2000u;
    sys_time.wMonth = date_time.month;
    sys_time.wDay = date_time.day;
    sys_time.wHour = date_time.hour;
    sys_time.wMinute = date_time.minute;
    sys_time.wSecond = date_time.second;
    sys_time.wMilliseconds = date_time.millis;

    // Set system time
    return (SetLocalTime(&sys_time) != 0);
}

/** \brief Get the date and time in the RTC */
bool WindowsRtc::getDateTime(DateTime& date_time)
{
    // Get system time
    SYSTEMTIME sys_time;
    GetLocalTime(&sys_time);

    // Convert to DateTime
    date_time.year = sys_time.wYear - 2000u;
    date_time.month = sys_time.wMonth;
    date_time.day = sys_time.wDay;
    date_time.hour = sys_time.wHour;
    date_time.minute = sys_time.wMinute;
    date_time.second = sys_time.wSecond;
    date_time.millis = sys_time.wMilliseconds;

    return true;
}


}

