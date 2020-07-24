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

#ifndef IGNSS_H
#define IGNSS_H

#include <cstdint>
#include "IRtc.h"

namespace open_vario
{



/** \brief Interface for all global navigation satellite systems implementations */
class IGnss
{
    public:


        /** \brief GNSS navigation data */
        struct NavigationData
        {
            /** \brief Date and time (UTC) */
            IRtc::DateTime date_time;
            /** \brief Latitude (1 = 1°) */
            double latitude;
            /** \brief Longitude (1 = 1°) */
            double longitude;
            /** \brief Speed (1 = 0.1 m/s) */
            uint32_t speed;
            /** \brief Altitude (1 = 0.1 m) */
            uint32_t altitude;
            /** \brief Track angle (1 = 0.1°) */
            uint16_t track_angle;
            /** \brief Number of satellites */
            uint8_t satellite_count;
            /** \brief Horizontal precision (m) */
            double hrms;
            /** \brief Vertical precision (m) */
            double vrms;
            /** \brief Horizontal dilution of precision */
            double hdop;
            /** \brief Vertical dilution of precision */
            double vdop;
        };

        /** \brief Check if the GNSS hardware is present and working */
        virtual bool probe() = 0;

        /** \brief Configure the GNSS */
        virtual bool configure(const uint16_t acqu_period) = 0;

        /** \brief Start the GNSS */
        virtual bool start() = 0;

        /** \brief Stop the GNSS */
        virtual bool stop() = 0;

        /** \brief Read the current navigation data */
        virtual bool readData(NavigationData& nav_data) = 0;
};

}

#endif // IGNSS_H
