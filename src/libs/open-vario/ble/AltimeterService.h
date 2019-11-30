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

#ifndef ALTIMETERSERVICE_H
#define ALTIMETERSERVICE_H

#include "OpenVarioBleServiceBase.h"

#include "BleService.h"
#include "BleCharacteristic.h"

#include "Delegate.h"
#include "IAltimeter.h"


namespace open_vario
{


/** \brief BLE altimeter service */
class AltimeterService : public OpenVarioBleServiceBase, public IBleCharacteristicListener
{
    public:

        /** \brief Constructor */
        AltimeterService();


        /** \brief Initialize the BLE service */
        virtual bool init();

        /** \brief Start the BLE service */
        virtual bool start();

        /** \brief Update the BLE service characteristics values */
        virtual void updateCharacteristicsValues();

        /** \brief Get the BLE service */
        virtual IBleService& getService() { return m_altimeter_service; } 


        /** \brief Called when the characteristic's value has changed */
        virtual void onValueChanged(IBleCharacteristic& characteristic, const bool from_stack, const void* new_value, const uint16_t new_value_size);
   

    private:


        /** \brief Commands */
        enum Command
        {
            /** \brief Set the main altitude */
            CMD_SET_MAIN_ALTI = 0x1000u,
            /** \brief Set the altitude 1 */
            CMD_SET_ALTI_1 = 0x1001u,
            /** \brief Set the altitude 2 */
            CMD_SET_ALTI_2 = 0x1002u,
            /** \brief Set the altitude 3 */
            CMD_SET_ALTI_3 = 0x1003u,
            /** \brief Set the altitude 4 */
            CMD_SET_ALTI_4 = 0x1004u
        };


        /** \brief Altimeter service */
        BleService128<0u, 2u> m_altimeter_service;

        /** \brief Altitudes */
        BleCharacteristic128<uint8_t*, 0u> m_altitudes;

        /** \brief Command */
        BleCharacteristic128<uint32_t, 0u> m_command;


        /** \brief Altimeter event handler */
        nano_stl::IEvent<const AltimeterValues&>::EventHandlerM m_altimeter_event_handler;


        /** \brief Altimeter values */
        AltimeterValues m_altimeter_values;



        /** \brief Called when new altimeter values have been computed */
        void onAltimeterValues(const AltimeterValues& alti_values);

};

}

#endif // ALTIMETERSERVICE_H
