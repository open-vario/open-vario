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

#ifndef IMCURTC_H
#define IMCURTC_H

#include <cstdint>

namespace open_vario
{



/** \brief Interface for all MCU RTC implementations */
class IMcuRtc
{
    public:

        /** \brief Date and time */
        struct DateTime
        {
            /** \brief Year (since 2000) */
            uint8_t year;
            /** \brief Month (1 - 12) */
            uint8_t month;
            /** \brief Day (1 - 31) */
            uint8_t day;
            /** \brief Hour (0 - 24) */
            uint8_t hour;
            /** \brief Minute (0 - 59) */
            uint8_t minute;
            /** \brief Second (0 - 59) */
            uint8_t second;
        };

        /** \brief Configure the RTC */
        virtual bool configure() = 0;

        /** \brief Set the date and time in the RTC */
        virtual bool setDateTime(const DateTime& date_time) = 0;

        /** \brief Get the date and time in the RTC */
        virtual bool getDateTime(DateTime& date_time) = 0;

};

}

#endif // IMCURTC_H
