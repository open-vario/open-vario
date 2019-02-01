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

#ifndef NAVIGATIONSERVICE_H
#define NAVIGATIONSERVICE_H

#include "OpenVarioBleServiceBase.h"

#include "BleService.h"
#include "BleCharacteristic.h"

#include "Delegate.h"



namespace open_vario
{


/** \brief BLE navigation service */
class NavigationService : public OpenVarioBleServiceBase
{
    public:

        /** \brief Constructor */
        NavigationService();


        /** \brief Initialize the BLE service */
        virtual bool init();

        /** \brief Start the BLE service */
        virtual bool start();

        /** \brief Update the BLE service characteristics values */
        virtual void updateCharacteristicsValues();

        /** \brief Get the BLE service */
        virtual IBleService& getService() { return m_navigation_service; } 
   

    private:


        /** \brief Navigation service */
        BleService128<0u, 4u> m_navigation_service;

        /** \brief Speed */
        BleCharacteristic128<uint16_t, 0u> m_speed;

        /** \brief Latitude */
        BleCharacteristic128<double, 0u> m_latitude;

        /** \brief Longitude */
        BleCharacteristic128<double, 0u> m_longitude;

        /** \brief Track angle */
        BleCharacteristic128<uint16_t, 0u> m_track_angle;

};

}

#endif // NAVIGATIONSERVICE_H
