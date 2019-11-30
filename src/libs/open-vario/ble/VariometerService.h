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

#ifndef VARIOMETERSERVICE_H
#define VARIOMETERSERVICE_H

#include "OpenVarioBleServiceBase.h"

#include "BleService.h"
#include "BleCharacteristic.h"

#include "Delegate.h"
#include "IVariometer.h"


namespace open_vario
{


/** \brief BLE variometer service */
class VariometerService : public OpenVarioBleServiceBase
{
    public:

        /** \brief Constructor */
        VariometerService();


        /** \brief Initialize the BLE service */
        virtual bool init();

        /** \brief Start the BLE service */
        virtual bool start();

        /** \brief Update the BLE service characteristics values */
        virtual void updateCharacteristicsValues();

        /** \brief Get the BLE service */
        virtual IBleService& getService() { return m_variometer_service; } 
   

    private:


        /** \brief Variometer service */
        BleService128<0u, 1u> m_variometer_service;

        /** \brief Vario and acceleration*/
        BleCharacteristic128<uint32_t, 0u> m_vario_accel;


        /** \brief Variometer event handler */
        nano_stl::IEvent<const VariometerValues&>::EventHandlerM m_variometer_event_handler;


        /** \brief Variometer values */
        VariometerValues m_variometer_values;



        /** \brief Called when new variometer values have been computed */
        void onVariometerValues(const VariometerValues& vario_values);

};

}

#endif // VARIOMETERSERVICE_H
