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

#ifndef FLIGHTDATASERVICE_H
#define FLIGHTDATASERVICE_H

#include "IOpenVarioBleService.h"

#include "BleService.h"
#include "BleCharacteristic.h"

#include "BarometerService.h"
#include "AltimeterService.h"
#include "VariometerService.h"


namespace open_vario
{


/** \brief BLE flight data service */
class FlightDataService : public IOpenVarioBleService
{
    public:

        /** \brief Constructor */
        FlightDataService();


        /** \brief Initialize the BLE service */
        virtual bool init();

        /** \brief Start the BLE service */
        virtual bool start();

        /** \brief Update the BLE service characteristics values */
        virtual void updateCharacteristicsValues();

        /** \brief Get the BLE service */
        virtual IBleService& getService() { return m_flight_data_service; } 
   

    private:


        /** \brief Flight data service */
        BleService128<4u, 0u> m_flight_data_service;

        /** \brief Barometer service */
        BarometerService m_barometer_service;

        /** \brief Altimeter service */
        AltimeterService m_altimeter_service;

        /** \brief Variometer service */
        VariometerService m_variometer_service;

};

}

#endif // FLIGHTDATASERVICE_H
