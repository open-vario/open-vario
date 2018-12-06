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

#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include "IRtc.h"

namespace open_vario
{


/** \brief Date and time manager */
class TimeManager
{
    public:

        /** \brief Constructor */
        TimeManager(IRtc& rtc);


        /** \brief Start the date and time manager */
        bool start();

        /** \brief Set the system's date and time */
        bool setDateTime(const IRtc::DateTime& date_time);

        /** \brief Get the system's date and time */
        bool getDateTime(IRtc::DateTime& date_time);

        

    private:

        /** \brief RTC peripheral */
        IRtc& m_rtc;
};

}

#endif // TIMEMANAGER_H
