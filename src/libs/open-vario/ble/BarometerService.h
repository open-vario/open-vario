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

#ifndef BAROMETERSERVICE_H
#define BAROMETERSERVICE_H

#include "OpenVarioBleServiceBase.h"

#include "BleService.h"
#include "BleCharacteristic.h"

#include "Delegate.h"
#include "IBarometer.h"
#include "IThermometer.h"


namespace open_vario
{


/** \brief BLE barometer service */
class BarometerService : public OpenVarioBleServiceBase
{
    public:

        /** \brief Constructor */
        BarometerService();


        /** \brief Initialize the BLE service */
        virtual bool init();

        /** \brief Start the BLE service */
        virtual bool start();

        /** \brief Update the BLE service characteristics values */
        virtual void updateCharacteristicsValues();

        /** \brief Get the BLE service */
        virtual IBleService& getService() { return m_barometer_service; }
   

    private:


        /** \brief Barometer service */
        BleService128<0u, 2u> m_barometer_service;

        /** \brief Pressure and Temperature*/
        BleCharacteristic128<uint8_t*, 0u> m_pressure_temperature;

        /** \brief Pressure and Temperature min and max values*/
        BleCharacteristic128<uint8_t*, 0u> m_min_max;


        /** \brief Barometer event handler */
        nano_stl::IEvent<const BarometerValues&>::EventHandlerM m_barometer_event_handler;

        /** \brief Thermometer event handler */
        nano_stl::IEvent<const ThermometerValues&>::EventHandlerM m_thermometer_event_handler;


        /** \brief Barometer values */
        BarometerValues m_barometer_values;

        /** \brief Thermometer values */
        ThermometerValues m_thermometer_values;



        /** \brief Called when new barometer values have been computed */
        void onBarometerValues(const BarometerValues& baro_values);

        /** \brief Called when new thermometer values have been computed */
        void onThermometerValues(const ThermometerValues& temp_values);

};

}

#endif // BAROMETERSERVICE_H
