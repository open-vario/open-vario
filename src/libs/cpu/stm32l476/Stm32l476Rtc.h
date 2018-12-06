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

#ifndef STM32L476RTC_H
#define STM32L476RTC_H

#include "IMcuRtc.h"

namespace open_vario
{


/** \brief STM32L476 RTC */
class Stm32l476Rtc : public IMcuRtc
{
    public:

        /** \brief Constructor */
        Stm32l476Rtc();


        /** \brief Configure the RTC */
        virtual bool configure();

        /** \brief Set the date and time in the RTC */
        virtual bool setDateTime(const DateTime& date_time);

        /** \brief Get the date and time in the RTC */
        virtual bool getDateTime(DateTime& date_time);


    private:

        /** \brief Synchronize shadow registers */
        void syncShadowRegs();
 
        /** \brief Convert a value to BCD format */
        uint8_t toBcd(const uint8_t value);

        /** \brief Convert a value from BCD format */
        uint8_t fromBcd(const uint8_t bcd_value);
};

}

#endif // STM32L476RTC_H
