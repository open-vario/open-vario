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

#include "FlightDataService.h"

namespace open_vario
{


/** \brief Constructor */
FlightDataService::FlightDataService()
: m_flight_data_service("Flight data service", {0x7bu, 0xb0u, 0x55u, 0xf2u, 0xabu, 0x96u, 0x43u, 0xabu, 0x9eu, 0xd4u, 0xf8u, 0xdbu, 0xaeu, 0xc1u, 0xafu, 0x10u})

, m_barometer_service()
, m_altimeter_service()
, m_variometer_service()
{}


/** \brief Initialize the BLE service */
bool FlightDataService::init()
{
    bool ret = true;

    // Add included services
    ret = ret && m_flight_data_service.addService(m_barometer_service.getService());
    ret = ret && m_flight_data_service.addService(m_altimeter_service.getService());
    ret = ret && m_flight_data_service.addService(m_variometer_service.getService());
    if (ret)
    {
        // Initialize included services
        ret = ret && m_barometer_service.init();
        ret = ret && m_altimeter_service.init();
        ret = ret && m_variometer_service.init();
    }

    return ret;
}

/** \brief Start the BLE service */
bool FlightDataService::start()
{
    bool ret = true;

    // Start included services
    ret = ret && m_barometer_service.start();
    ret = ret && m_altimeter_service.start();
    ret = ret && m_variometer_service.start();
    
    return ret;
}

/** \brief Update the BLE service characteristics values */
void FlightDataService::updateCharacteristicsValues()
{
    // Update included services
    m_barometer_service.updateCharacteristicsValues();
    m_altimeter_service.updateCharacteristicsValues();
    m_variometer_service.updateCharacteristicsValues();
}


}
